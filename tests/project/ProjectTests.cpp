#include "FileSystem/EnginePath.h"
#include "FileSystem/FileSystem.h"
#include "Project/ProjectManager.h"

#include <filesystem>
#include <gtest/gtest.h>
#include <string>

namespace
{
class ProjectTest : public ::testing::Test
{
protected:
  void SetUp() override
  {
    if (!psych::Filesystem::IsInitialized()) {
      psych::Filesystem::Init();
    }

    const auto* testInfo = ::testing::UnitTest::GetInstance()->current_test_info();
    m_Root_              = std::filesystem::temp_directory_path() / ("PSYCH_ENGINE_project_tests_" + std::string(testInfo->name()));
    std::filesystem::remove_all(m_Root_);
    std::filesystem::create_directories(m_Root_);
    psych::Filesystem::ClearProjectRoot();
  }

  void TearDown() override
  {
    psych::Filesystem::ClearProjectRoot();
    std::filesystem::remove_all(m_Root_);
  }

  std::filesystem::path m_Root_;
};

TEST_F(ProjectTest, OpenProjectBindsLogicalRootAndCreatesMetadata)
{
  psych::ProjectManager manager;
  ASSERT_TRUE(manager.Init());
  ASSERT_TRUE(manager.OpenProject(m_Root_));

  ASSERT_TRUE(manager.HasActiveProject());
  EXPECT_EQ(manager.GetActiveProject().GetProjectPath().string(), "proj://");
  EXPECT_EQ(manager.GetActiveProject().GetAssetRootPath().string(), "proj://data");
  EXPECT_TRUE(psych::Filesystem::FileExists(psych::EnginePath::Path{"proj://project.xml"}));
  EXPECT_TRUE(psych::Filesystem::DirExists(psych::EnginePath::Path{"proj://data"}));

  const auto configContents = psych::Filesystem::TryReadFile(psych::EnginePath::Path{"proj://project.xml"});
  ASSERT_TRUE(configContents);
  EXPECT_NE(configContents->find("proj://data"), std::string::npos);

  EXPECT_TRUE(manager.Shutdown());
  EXPECT_FALSE(psych::Filesystem::TryResolve(psych::EnginePath::Path{"proj://project.xml"}));
}

TEST_F(ProjectTest, OpeningAnotherProjectRebindsProjectSchema)
{
  const auto firstRoot  = m_Root_ / "first";
  const auto secondRoot = m_Root_ / "second";
  std::filesystem::create_directories(firstRoot);
  std::filesystem::create_directories(secondRoot);

  psych::ProjectManager manager;
  ASSERT_TRUE(manager.Init());
  ASSERT_TRUE(manager.OpenProject(firstRoot));
  ASSERT_TRUE(psych::Filesystem::TryWriteFile(psych::EnginePath::Path{"proj://data/marker.txt"}, "first"));

  ASSERT_TRUE(manager.OpenProject(secondRoot));
  ASSERT_TRUE(psych::Filesystem::TryWriteFile(psych::EnginePath::Path{"proj://data/marker.txt"}, "second"));

  EXPECT_EQ(psych::Filesystem::StreamFile(psych::EnginePath::Path{"proj://data/marker.txt"}), "second");
  EXPECT_TRUE(std::filesystem::is_regular_file(firstRoot / "data" / "marker.txt"));
  EXPECT_TRUE(std::filesystem::is_regular_file(secondRoot / "data" / "marker.txt"));

  EXPECT_TRUE(manager.Shutdown());
}
} // namespace
