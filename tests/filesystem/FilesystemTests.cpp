#include "FileSystem/CoreFilesystemAPI.h"
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
    const auto* testInfo = ::testing::UnitTest::GetInstance()->current_test_info();
    m_Root_ = std::filesystem::temp_directory_path() / ("PSYCH_ENGINE_fs_tests_" + std::string(testInfo->test_suite_name()) + "_" + std::string(testInfo->name()));
    std::filesystem::remove_all(m_Root_);
    std::filesystem::create_directories(m_Root_);
  }

  void TearDown() override { std::filesystem::remove_all(m_Root_); }

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

TEST_F(FilesystemTest, PlatformConfigPathRequiresFilesystemInitialization)
{
  psych::Filesystem::Shutdown();

  const auto result = psych::Filesystem::TryGetBaseConfigPath();

  ASSERT_FALSE(result);
  EXPECT_EQ(result.error(), psych::errors::FilesystemError::NotInitialized);
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

  psych::Unique<psych::FileNode> root = psych::Filesystem::CreateDirectoryTree(m_Root_);

  ASSERT_NE(root, nullptr);
  EXPECT_TRUE(root->isDir);
  EXPECT_EQ(root->path, m_Root_);
  EXPECT_EQ(root->name, m_Root_.filename().string());
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
  ASSERT_EQ(childDirectory->children.size(), 1u);
  EXPECT_EQ(childDirectory->children.front()->name, "nested.txt");
  EXPECT_FALSE(childDirectory->children.front()->isDir);
}

TEST_F(FilesystemTest, TryCreateDirectoryTreeReportsMissingRoot)
{
  const auto result = psych::Filesystem::TryCreateDirectoryTree(m_Root_ / "missing");

  ASSERT_FALSE(result);
  EXPECT_EQ(result.error(), psych::errors::FilesystemError::FileNotFound);
}
} // namespace
