#ifndef SIMPLE_TUNER_PLATFORM_IOS_IOS_CONFIG_STORAGE_H_
#define SIMPLE_TUNER_PLATFORM_IOS_IOS_CONFIG_STORAGE_H_

#include <optional>
#include <string>

#include "simple_tuner/interfaces/IConfigStorage.h"

namespace simple_tuner {

class iOSConfigStorage : public IConfigStorage {
 public:
  iOSConfigStorage();
  ~iOSConfigStorage() override;

  bool set_double(const std::string& key, double value) noexcept override;
  std::optional<double> get_double(
      const std::string& key) const noexcept override;
  bool remove(const std::string& key) noexcept override;
  void clear() noexcept override;

  iOSConfigStorage(const iOSConfigStorage&) = delete;
  iOSConfigStorage& operator=(const iOSConfigStorage&) = delete;
  iOSConfigStorage(iOSConfigStorage&&) = delete;
  iOSConfigStorage& operator=(iOSConfigStorage&&) = delete;

 private:
  static constexpr const char* kKeyPrefix = "com.simpletuner.";
};

}  // namespace simple_tuner

#endif  // SIMPLE_TUNER_PLATFORM_IOS_IOS_CONFIG_STORAGE_H_
