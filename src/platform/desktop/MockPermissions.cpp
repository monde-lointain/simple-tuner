#include "simple_tuner/platform/desktop/MockPermissions.h"

namespace simple_tuner {

PermissionStatus MockPermissions::get_microphone_status() const noexcept {
  return PermissionStatus::kGranted;
}

void MockPermissions::request_microphone_permission(
    std::function<void(PermissionStatus)> callback) noexcept {
  if (callback) {
    callback(PermissionStatus::kGranted);
  }
}

}  // namespace simple_tuner
