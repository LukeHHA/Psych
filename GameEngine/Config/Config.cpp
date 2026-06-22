#include "Config.h"
#include "Debug/Assert.h"
#include "FileSystem/FileSystem.h"
#include "FileSystem/OSFilesystemAPI.h"
#include <iostream>
#include <pugixml.hpp>

namespace ge
{
Config::Config()
{
  pugi::xml_document doc;
  auto configPath = util::Filesystem::TryGetBaseConfigPath();
  if (!configPath) {
    CORE_ASSERT(false, "Failed to get config directory");
    return;
  }

  const FilePath configFile = configPath.value() / "config-test.xml";
  if (!util::Filesystem::FileExists(configFile)) {
    if (util::Filesystem::TryCreateFile(configFile)) {
      std::cout << "File created!" << std::endl;
    } else {
      CORE_ASSERT(false, "Failed to create file");
    }
  }
}

Config::~Config() {}
} // namespace ge
