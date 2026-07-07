#include "MacOSFilesystemAPI.h"

#import <Foundation/Foundation.h>

#include <filesystem>
#include <stdexcept>

namespace psych
{
std::filesystem::path MacOSFilesystemAPI::GetApplicationSupportPath() const
{
  @autoreleasepool {
    NSFileManager* fileManager = [NSFileManager defaultManager];

    NSArray<NSURL*>* urls =
        [fileManager URLsForDirectory:NSApplicationSupportDirectory
                            inDomains:NSUserDomainMask];

    if ([urls count] == 0) {
      throw std::runtime_error("Could not find Application Support directory");
    }

    NSURL* appSupportURL = urls[0];

    return std::filesystem::path([[appSupportURL path] UTF8String]);
  }
}

#import <Foundation/Foundation.h>

#include <filesystem>
#include <string>

std::filesystem::path MacOSFilesystemAPI::GetCachePath() const
{
  @autoreleasepool {
    NSFileManager* fileManager = [NSFileManager defaultManager];

    NSURL* url                 = [fileManager URLForDirectory:NSCachesDirectory
                                     inDomain:NSUserDomainMask
                            appropriateForURL:nil
                                       create:YES
                                        error:nil];

    if (!url) {
      return {};
    }

    return std::filesystem::path([[url path] UTF8String]);
  }
}
} // namespace psych
