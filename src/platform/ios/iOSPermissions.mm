#include "simple_tuner/platform/ios/iOSPermissions.h"

#import <AVFoundation/AVFoundation.h>
#import <juce_events/juce_events.h>

namespace simple_tuner {

iOSPermissions::iOSPermissions() = default;

iOSPermissions::~iOSPermissions() = default;

PermissionStatus iOSPermissions::get_microphone_status() const noexcept {
  @try {
    AVAudioSessionRecordPermission permission =
        [[AVAudioSession sharedInstance] recordPermission];

    switch (permission) {
      case AVAudioSessionRecordPermissionGranted:
        return PermissionStatus::kGranted;
      case AVAudioSessionRecordPermissionDenied:
        return PermissionStatus::kDenied;
      case AVAudioSessionRecordPermissionUndetermined:
        return PermissionStatus::kNotDetermined;
      default:
        return PermissionStatus::kNotDetermined;
    }
  } @catch (NSException* exception) {
    NSLog(@"Exception in get_microphone_status: %@", exception);
    return PermissionStatus::kNotDetermined;
  } catch (...) {
    return PermissionStatus::kNotDetermined;
  }
}

void iOSPermissions::request_microphone_permission(
    std::function<void(PermissionStatus)> callback) noexcept {
  @try {
    [[AVAudioSession sharedInstance]
        requestRecordPermission:^(BOOL granted) {
      PermissionStatus status = granted ? PermissionStatus::kGranted
                                        : PermissionStatus::kDenied;

      if (callback) {
        // Dispatch to main thread using JUCE
        juce::MessageManager::callAsync([callback, status]() {
          callback(status);
        });
      }
    }];
  } @catch (NSException* exception) {
    NSLog(@"Exception in request_microphone_permission: %@", exception);
    if (callback) {
      juce::MessageManager::callAsync([callback]() {
        callback(PermissionStatus::kDenied);
      });
    }
  } catch (...) {
    if (callback) {
      try {
        juce::MessageManager::callAsync([callback]() {
          callback(PermissionStatus::kDenied);
        });
      } catch (...) {
        // Suppress nested exceptions
      }
    }
  }
}

}  // namespace simple_tuner
