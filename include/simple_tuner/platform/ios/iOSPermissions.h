#ifndef SIMPLE_TUNER_PLATFORM_IOS_IOS_PERMISSIONS_H_
#define SIMPLE_TUNER_PLATFORM_IOS_IOS_PERMISSIONS_H_

#include "simple_tuner/interfaces/IPermissions.h"

namespace simple_tuner {

class iOSPermissions : public IPermissions {
 public:
  iOSPermissions();
  ~iOSPermissions() override;

  PermissionStatus get_microphone_status() const noexcept override;
  void request_microphone_permission(
      void (*callback)(PermissionStatus)) noexcept override;

  iOSPermissions(const iOSPermissions&) = delete;
  iOSPermissions& operator=(const iOSPermissions&) = delete;
  iOSPermissions(iOSPermissions&&) = delete;
  iOSPermissions& operator=(iOSPermissions&&) = delete;
};

}  // namespace simple_tuner

#endif  // SIMPLE_TUNER_PLATFORM_IOS_IOS_PERMISSIONS_H_
