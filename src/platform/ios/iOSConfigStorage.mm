#include "simple_tuner/platform/ios/iOSConfigStorage.h"

#import <Foundation/Foundation.h>

namespace simple_tuner {

iOSConfigStorage::iOSConfigStorage() = default;

iOSConfigStorage::~iOSConfigStorage() = default;

bool iOSConfigStorage::set_double(const std::string& key,
                                   double value) noexcept {
  @try {
    std::string prefixed_key = kKeyPrefix + key;
    NSString* ns_key =
        [NSString stringWithUTF8String:prefixed_key.c_str()];

    if (ns_key == nil) {
      return false;
    }

    [[NSUserDefaults standardUserDefaults] setDouble:value forKey:ns_key];
    [[NSUserDefaults standardUserDefaults] synchronize];

    return true;
  } @catch (NSException* exception) {
    NSLog(@"Exception in set_double: %@", exception);
    return false;
  } catch (...) {
    return false;
  }
}

std::optional<double> iOSConfigStorage::get_double(
    const std::string& key) const noexcept {
  @try {
    std::string prefixed_key = kKeyPrefix + key;
    NSString* ns_key =
        [NSString stringWithUTF8String:prefixed_key.c_str()];

    if (ns_key == nil) {
      return std::nullopt;
    }

    NSUserDefaults* defaults = [NSUserDefaults standardUserDefaults];
    id value = [defaults objectForKey:ns_key];

    if (value == nil) {
      return std::nullopt;
    }

    double result = [defaults doubleForKey:ns_key];
    return result;
  } @catch (NSException* exception) {
    NSLog(@"Exception in get_double: %@", exception);
    return std::nullopt;
  } catch (...) {
    return std::nullopt;
  }
}

bool iOSConfigStorage::remove(const std::string& key) noexcept {
  @try {
    std::string prefixed_key = kKeyPrefix + key;
    NSString* ns_key =
        [NSString stringWithUTF8String:prefixed_key.c_str()];

    if (ns_key == nil) {
      return false;
    }

    [[NSUserDefaults standardUserDefaults] removeObjectForKey:ns_key];
    [[NSUserDefaults standardUserDefaults] synchronize];

    return true;
  } @catch (NSException* exception) {
    NSLog(@"Exception in remove: %@", exception);
    return false;
  } catch (...) {
    return false;
  }
}

void iOSConfigStorage::clear() noexcept {
  @try {
    NSUserDefaults* defaults = [NSUserDefaults standardUserDefaults];
    NSDictionary* dict = [defaults dictionaryRepresentation];

    NSString* prefix = [NSString stringWithUTF8String:kKeyPrefix];
    if (prefix == nil) {
      return;
    }

    for (NSString* key in dict) {
      if ([key hasPrefix:prefix]) {
        [defaults removeObjectForKey:key];
      }
    }

    [defaults synchronize];
  } @catch (NSException* exception) {
    NSLog(@"Exception in clear: %@", exception);
  } catch (...) {
    // Suppress exceptions
  }
}

}  // namespace simple_tuner
