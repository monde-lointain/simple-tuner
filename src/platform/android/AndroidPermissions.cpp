#include "simple_tuner/platform/android/AndroidPermissions.h"

#include <juce_core/juce_core.h>
#include <juce_events/juce_events.h>

namespace simple_tuner {

AndroidPermissions::AndroidPermissions() = default;

AndroidPermissions::~AndroidPermissions() = default;

PermissionStatus AndroidPermissions::get_microphone_status() const noexcept {
  try {
    if (juce::RuntimePermissions::isGranted(
            juce::RuntimePermissions::recordAudio)) {
      return PermissionStatus::kGranted;
    }

    // JUCE doesn't distinguish between denied and not determined on Android
    return PermissionStatus::kNotDetermined;
  } catch (...) {
    return PermissionStatus::kNotDetermined;
  }
}

void AndroidPermissions::request_microphone_permission(
    void (*callback)(PermissionStatus)) noexcept {
  try {
    juce::RuntimePermissions::request(
        juce::RuntimePermissions::recordAudio, [callback](bool granted) {
          PermissionStatus status =
              granted ? PermissionStatus::kGranted : PermissionStatus::kDenied;

          if (callback != nullptr) {
            // Dispatch to main thread using JUCE
            juce::MessageManager::callAsync(
                [callback, status]() { callback(status); });
          }
        });
  } catch (...) {
    if (callback != nullptr) {
      try {
        juce::MessageManager::callAsync(
            [callback]() { callback(PermissionStatus::kDenied); });
      } catch (...) {
        // Suppress nested exceptions
      }
    }
  }
}

}  // namespace simple_tuner
