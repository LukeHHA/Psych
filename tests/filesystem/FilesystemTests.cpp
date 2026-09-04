#include "FileSystem/CoreFilesystemAPI.h"
#include "FileSystem/EnginePath.h"
#include "FileSystem/FileSystem.h"
#include <algorithm>
#include <filesystem>
#include <fstream>
#include <gtest/gtest.h>
#include <string>
#include <vector>

namespace
{
class FilesystemTest : public ::testing::Test
{
protected:
  void SetUp() override
  {
    if (!psych::Filesystem::IsInitialized()) {
      psych::Filesystem::Init();
    }

    const auto* testInfo = ::testing::UnitTest::GetInstance()->current_test_info();
    m_Root_ = std::filesystem::temp_directory_path() / ("PSYCH_ENGINE_fs_tests_" + std::string(testInfo->test_suite_name()) + "_" + std::string(testInfo->name()));
    std::filesystem::remove_all(m_Root_);
    std::filesystem::create_directories(m_Root_);
  }

  void TearDown() override
  {
    psych::Filesystem::ClearProjectRoot();
    std::filesystem::remove_all(m_Root_);
  }

  std::filesystem::path m_Root_;
};

TEST_F(FilesystemTest, CoreFilesystemDetectsFilesAndDirectories)
{
  const std::filesystem::path directory = m_Root_ / "assets";
  const std::filesystem::path file      = directory / "scene.txt";

  ASSERT_TRUE(psych::CoreFilesystemAPI::CreateDirs(directory));
  {
    std::ofstream output(file);
    output << "scene-data";
  }

  EXPECT_TRUE(psych::CoreFilesystemAPI::DirExists(directory));
  EXPECT_FALSE(psych::CoreFilesystemAPI::DirExists(file));
  EXPECT_TRUE(psych::CoreFilesystemAPI::FileExists(file));
  EXPECT_FALSE(psych::CoreFilesystemAPI::FileExists(directory));
  EXPECT_EQ(psych::CoreFilesystemAPI::StreamFile(file.string()), "scene-data");

  const auto readResult = psych::CoreFilesystemAPI::TryReadFile(file);
  ASSERT_TRUE(readResult);
  EXPECT_EQ(readResult.value(), "scene-data");
}

TEST_F(FilesystemTest, DeleteFileRemovesOnlyExistingRegularFiles)
{
  const std::filesystem::path file = m_Root_ / "delete-me.txt";
  {
    std::ofstream output(file);
    output << "temporary";
  }

  ASSERT_TRUE(psych::CoreFilesystemAPI::FileExists(file));
  psych::CoreFilesystemAPI::DeleteFile(file);
  EXPECT_FALSE(psych::CoreFilesystemAPI::FileExists(file));

  EXPECT_NO_THROW(psych::CoreFilesystemAPI::DeleteFile(file));
}

TEST_F(FilesystemTest, TryDeleteFileReportsWrongTypeWhenPathIsDirectory)
{
  const std::filesystem::path directory = m_Root_ / "delete-dir";
  std::filesystem::create_directories(directory);

  const auto result = psych::CoreFilesystemAPI::TryDeleteFile(directory);

  ASSERT_FALSE(result);
  EXPECT_EQ(result.error(), psych::errors::FilesystemError::PathExistsWithWrongType);
}

TEST_F(FilesystemTest, TryReadFileReportsMissingFiles)
{
  const std::filesystem::path missingFile = m_Root_ / "missing.txt";

  const auto result                       = psych::CoreFilesystemAPI::TryReadFile(missingFile);

  ASSERT_FALSE(result);
  EXPECT_EQ(result.error(), psych::errors::FilesystemError::FileNotFound);
}

TEST_F(FilesystemTest, TryCreateFileCreatesParentDirectories)
{
  const std::filesystem::path file = m_Root_ / "config" / "settings.xml";

  const auto result                = psych::CoreFilesystemAPI::TryCreateFile(file);

  ASSERT_TRUE(result);
  EXPECT_TRUE(psych::CoreFilesystemAPI::FileExists(file));
}

TEST_F(FilesystemTest, TryCreateFileReportsWrongTypeWhenPathIsDirectory)
{
  const std::filesystem::path directory = m_Root_ / "config";
  std::filesystem::create_directories(directory);

  const auto result = psych::CoreFilesystemAPI::TryCreateFile(directory);

  ASSERT_FALSE(result);
  EXPECT_EQ(result.error(), psych::errors::FilesystemError::PathExistsWithWrongType);
}

TEST_F(FilesystemTest, PlatformPathsRequireFilesystemInitialization)
{
  psych::Filesystem::Shutdown();

  const auto configResult = psych::Filesystem::TryGetBaseConfigPath();
  const auto logResult    = psych::Filesystem::TryGetBaseLogPath();

  ASSERT_FALSE(configResult);
  EXPECT_EQ(configResult.error(), psych::errors::FilesystemError::NotInitialized);
  ASSERT_FALSE(logResult);
  EXPECT_EQ(logResult.error(), psych::errors::FilesystemError::NotInitialized);
}

TEST_F(FilesystemTest, BaseLogPathUsesEngineLogsDirectory)
{
  const auto result = psych::Filesystem::TryGetBaseLogPath();

  ASSERT_TRUE(result);
  EXPECT_EQ(result->filename(), "Logs");
  EXPECT_EQ(result->parent_path().filename(), psych::PsychEngineName);
  EXPECT_TRUE(std::filesystem::is_directory(*result));
}

TEST_F(FilesystemTest, CreateDirectoryTreeBuildsFileAndDirectoryNodes)
{
  const std::filesystem::path rootFile  = m_Root_ / "root.txt";
  const std::filesystem::path childDir  = m_Root_ / "child";
  const std::filesystem::path childFile = childDir / "nested.txt";

  std::filesystem::create_directories(childDir);
  {
    std::ofstream output(rootFile);
    output << "root";
  }
  {
    std::ofstream output(childFile);
    output << "nested";
  }

  ASSERT_TRUE(psych::Filesystem::TrySetProjectRoot(m_Root_));
  psych::Unique<psych::FileNode> root = psych::Filesystem::CreateDirectoryTree(psych::EnginePath::Path{"proj://"});

  ASSERT_NE(root, nullptr);
  EXPECT_TRUE(root->isDir);
  EXPECT_EQ(root->path.string(), "proj://");
  EXPECT_TRUE(root->name.empty());
  ASSERT_EQ(root->children.size(), 2u);

  std::vector<std::string> childNames;
  for (const auto& child : root->children) {
    childNames.push_back(child->name);
  }
  std::sort(childNames.begin(), childNames.end());

  EXPECT_EQ(childNames, (std::vector<std::string>{"child", "root.txt"}));

  const psych::FileNode* childDirectory = nullptr;
  for (const auto& child : root->children) {
    if (child->name == "child") {
      childDirectory = child.get();
      break;
    }
  }

  ASSERT_NE(childDirectory, nullptr);
  EXPECT_TRUE(childDirectory->isDir);
  EXPECT_EQ(childDirectory->path.string(), "proj://child");
  ASSERT_EQ(childDirectory->children.size(), 1u);
  EXPECT_EQ(childDirectory->children.front()->name, "nested.txt");
  EXPECT_EQ(childDirectory->children.front()->path.string(), "proj://child/nested.txt");
  EXPECT_FALSE(childDirectory->children.front()->isDir);
}

TEST_F(FilesystemTest, TryCreateDirectoryTreeReportsMissingRoot)
{
  ASSERT_TRUE(psych::Filesystem::TrySetProjectRoot(m_Root_));
  const auto result = psych::Filesystem::TryCreateDirectoryTree(psych::EnginePath::Path{"proj://missing"});

  ASSERT_FALSE(result);
  EXPECT_EQ(result.error(), psych::errors::FilesystemError::FileNotFound);
}

TEST_F(FilesystemTest, ProjectPathsReadAndWriteThroughBoundRoot)
{
  ASSERT_TRUE(psych::Filesystem::TrySetProjectRoot(m_Root_));
  const psych::EnginePath::Path filePath{"proj://data/nested.txt"};

  ASSERT_TRUE(psych::Filesystem::TryWriteFile(filePath, "project-data"));

  const auto contents = psych::Filesystem::TryReadFile(filePath);
  ASSERT_TRUE(contents);
  EXPECT_EQ(contents.value(), "project-data");
  EXPECT_TRUE(std::filesystem::is_regular_file(m_Root_ / "data" / "nested.txt"));
}

TEST_F(FilesystemTest, ProjectPathsRequireBoundRoot)
{
  psych::Filesystem::ClearProjectRoot();

  const auto result = psych::Filesystem::TryResolve(psych::EnginePath::Path{"proj://data"});

  ASSERT_FALSE(result);
  EXPECT_EQ(result.error(), psych::errors::FilesystemError::InvalidPath);
}

TEST(EnginePathTest, RecognizesAndFormatsProjectPaths)
{
  const psych::EnginePath::Path path{"PrOj://data/scenes/intro.scene"};

  EXPECT_TRUE(path.IsValid());
  EXPECT_EQ(path.GetSchema(), psych::EnginePath::Schema::Project);
  EXPECT_EQ(path.GetRelativePath(), std::filesystem::path("data/scenes/intro.scene"));
  EXPECT_EQ(path.string(), "proj://data/scenes/intro.scene");
}

TEST(EnginePathTest, RejectsAbsoluteAndParentTraversalPaths)
{
  EXPECT_FALSE(psych::EnginePath::Path{"proj:///tmp/project"}.IsValid());
  EXPECT_FALSE(psych::EnginePath::Path{"proj://data/../project.xml"}.IsValid());
  EXPECT_TRUE(psych::EnginePath::Path{"proj://data/version..txt"}.IsValid());
}

TEST(EnginePathTest, AssignmentClearsPreviousValidStateOnInvalidInput)
{
  psych::EnginePath::Path path{"proj://data"};

  path = std::string{"missing-schema"};

  EXPECT_FALSE(path.IsValid());
  EXPECT_EQ(path.GetSchema(), psych::EnginePath::Schema::None);
  EXPECT_TRUE(path.GetRelativePath().empty());
}
} // namespace
