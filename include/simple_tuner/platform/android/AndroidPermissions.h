#ifndef SIMPLE_TUNER_PLATFORM_ANDROID_ANDROID_PERMISSIONS_H_
#define SIMPLE_TUNER_PLATFORM_ANDROID_ANDROID_PERMISSIONS_H_

#include "simple_tuner/interfaces/IPermissions.h"

namespace simple_tuner {

class AndroidPermissions : public IPermissions {
 public:
  AndroidPermissions();
  ~AndroidPermissions() override;

  PermissionStatus get_microphone_status() const noexcept override;
  void request_microphone_permission(
      std::function<void(PermissionStatus)> callback) noexcept override;

  AndroidPermissions(const AndroidPermissions&) = delete;
  AndroidPermissions& operator=(const AndroidPermissions&) = delete;
  AndroidPermissions(AndroidPermissions&&) = delete;
  AndroidPermissions& operator=(AndroidPermissions&&) = delete;
};

}  // namespace simple_tuner

#endif  // SIMPLE_TUNER_PLATFORM_ANDROID_ANDROID_PERMISSIONS_H_
