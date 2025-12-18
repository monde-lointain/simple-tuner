#include "simple_tuner/platform/PlatformFactory.h"

#include "simple_tuner/platform/common/ConfigStorageWrapper.h"

#if JUCE_IOS
#include "simple_tuner/platform/ios/iOSConfigStorage.h"
#include "simple_tuner/platform/ios/iOSPermissions.h"
#elif JUCE_ANDROID
#include "simple_tuner/platform/android/AndroidConfigStorage.h"
#include "simple_tuner/platform/android/AndroidPermissions.h"
#else
#include "simple_tuner/platform/desktop/MockConfigStorage.h"
#include "simple_tuner/platform/desktop/MockPermissions.h"
#endif

namespace simple_tuner {

std::unique_ptr<IConfigStorage> PlatformFactory::create_config_storage() {
#if JUCE_IOS
  return std::make_unique<iOSConfigStorage>();
#elif JUCE_ANDROID
  return std::make_unique<AndroidConfigStorage>();
#else
  return std::make_unique<MockConfigStorage>();
#endif
}

std::unique_ptr<IPermissions> PlatformFactory::create_permissions() {
#if JUCE_IOS
  return std::make_unique<iOSPermissions>();
#elif JUCE_ANDROID
  return std::make_unique<AndroidPermissions>();
#else
  return std::make_unique<MockPermissions>();
#endif
}

}  // namespace simple_tuner
