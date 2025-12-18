#ifndef SIMPLE_TUNER_PLATFORM_PLATFORM_FACTORY_H_
#define SIMPLE_TUNER_PLATFORM_PLATFORM_FACTORY_H_

#include <memory>

#include "simple_tuner/interfaces/IConfigStorage.h"
#include "simple_tuner/interfaces/IPermissions.h"

namespace simple_tuner {

// Factory for creating platform-specific implementations.
// Encapsulates platform detection logic to eliminate #if guards in client code.
class PlatformFactory {
 public:
  // Creates the appropriate IConfigStorage implementation for the current
  // platform.
  static std::unique_ptr<IConfigStorage> create_config_storage();

  // Creates the appropriate IPermissions implementation for the current
  // platform.
  static std::unique_ptr<IPermissions> create_permissions();

  PlatformFactory() = delete;
  ~PlatformFactory() = delete;
  PlatformFactory(const PlatformFactory&) = delete;
  PlatformFactory& operator=(const PlatformFactory&) = delete;
  PlatformFactory(PlatformFactory&&) = delete;
  PlatformFactory& operator=(PlatformFactory&&) = delete;
};

}  // namespace simple_tuner

#endif  // SIMPLE_TUNER_PLATFORM_PLATFORM_FACTORY_H_
