#include "MacOSFilesystemAPI.h"

#import <AppKit/NSOpenPanel.h>
#import <Foundation/Foundation.h>

#include <filesystem>
#include <stdexcept>

namespace psych
{
std::filesystem::path MacOSFilesystemAPI::GetApplicationSupportPath() const
{
  @autoreleasepool {
    NSFileManager* fileManager = [NSFileManager defaultManager];

    NSArray<NSURL*>* urls      = [fileManager URLsForDirectory:NSApplicationSupportDirectory inDomains:NSUserDomainMask];

    if ([urls count] == 0) {
      throw std::runtime_error("Could not find Application Support directory");
    }

    NSURL* appSupportURL = urls[0];

    return std::filesystem::path{[[appSupportURL path] UTF8String]};
  }
}

std::filesystem::path MacOSFilesystemAPI::GetCachePath() const
{
  @autoreleasepool {
    NSFileManager* fileManager = [NSFileManager defaultManager];

    NSURL* url                 = [fileManager URLForDirectory:NSCachesDirectory inDomain:NSUserDomainMask appropriateForURL:nil create:YES error:nil];

    if (url == nullptr) {
      return {};
    }

    return std::filesystem::path{[[url path] UTF8String]};
  }
}

std::filesystem::path MacOSFilesystemAPI::GetOSFileExplorer() const
{
  @autoreleasepool {
    NSOpenPanel* panel = [NSOpenPanel openPanel];

    [panel setCanChooseFiles:NO];
    [panel setCanChooseDirectories:YES];
    [panel setAllowsMultipleSelection:NO];
    [panel setMessage:@"Choose a project directory."];

    const NSModalResponse response = [panel runModal];
    if (response != NSModalResponseOK) {
      return {};
    }

    NSURL* url = [[panel URLs] firstObject];
    if (url == nil) {
      return {};
    }

    const char* selectedPath = [url fileSystemRepresentation];
    if (selectedPath == nullptr) {
      return {};
    }

    return std::filesystem::path{selectedPath};
  }
}
} // namespace psych
