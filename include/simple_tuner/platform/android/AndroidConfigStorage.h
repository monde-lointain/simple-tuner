#ifndef SIMPLE_TUNER_PLATFORM_ANDROID_ANDROID_CONFIG_STORAGE_H_
#define SIMPLE_TUNER_PLATFORM_ANDROID_ANDROID_CONFIG_STORAGE_H_

#include <memory>
#include <optional>
#include <string>

#include "simple_tuner/interfaces/IConfigStorage.h"

namespace juce {
class PropertiesFile;
}

namespace simple_tuner {

class AndroidConfigStorage : public IConfigStorage {
 public:
  AndroidConfigStorage();
  ~AndroidConfigStorage() override;

  bool set_double(const std::string& key, double value) noexcept override;
  std::optional<double> get_double(
      const std::string& key) const noexcept override;
  bool remove(const std::string& key) noexcept override;
  void clear() noexcept override;

  AndroidConfigStorage(const AndroidConfigStorage&) = delete;
  AndroidConfigStorage& operator=(const AndroidConfigStorage&) = delete;
  AndroidConfigStorage(AndroidConfigStorage&&) = delete;
  AndroidConfigStorage& operator=(AndroidConfigStorage&&) = delete;

 private:
  static constexpr const char* kPrefsName = "SimpleTunerPrefs";

  std::unique_ptr<juce::PropertiesFile> properties_;
};

}  // namespace simple_tuner

#endif  // SIMPLE_TUNER_PLATFORM_ANDROID_ANDROID_CONFIG_STORAGE_H_
