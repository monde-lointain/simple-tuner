#ifndef SIMPLE_TUNER_PLATFORM_COMMON_CONFIG_STORAGE_WRAPPER_H_
#define SIMPLE_TUNER_PLATFORM_COMMON_CONFIG_STORAGE_WRAPPER_H_

#include <memory>
#include <optional>
#include <string>

#include "simple_tuner/interfaces/IConfigStorage.h"

namespace simple_tuner {

// Template wrapper providing unified error handling for platform-specific
// storage backends. Wraps all operations in try-catch blocks to ensure
// noexcept compliance.
//
// Template parameter PlatformStorage must provide methods:
// - bool set_double(const std::string&, double)
// - std::optional<double> get_double(const std::string&) const
// - bool remove(const std::string&)
// - void clear()
template <typename PlatformStorage>
class ConfigStorageWrapper final : public IConfigStorage {
 public:
  explicit ConfigStorageWrapper(
      std::unique_ptr<PlatformStorage> storage) noexcept
      : storage_(std::move(storage)) {}

  ~ConfigStorageWrapper() override = default;

  // Disallow copy and move
  ConfigStorageWrapper(const ConfigStorageWrapper&) = delete;
  ConfigStorageWrapper& operator=(const ConfigStorageWrapper&) = delete;
  ConfigStorageWrapper(ConfigStorageWrapper&&) = delete;
  ConfigStorageWrapper& operator=(ConfigStorageWrapper&&) = delete;

  bool set_double(const std::string& key, double value) noexcept override {
    try {
      if (storage_ == nullptr) {
        return false;
      }
      return storage_->set_double(key, value);
    } catch (...) {
      return false;
    }
  }

  std::optional<double> get_double(
      const std::string& key) const noexcept override {
    try {
      if (storage_ == nullptr) {
        return std::nullopt;
      }
      return storage_->get_double(key);
    } catch (...) {
      return std::nullopt;
    }
  }

  bool remove(const std::string& key) noexcept override {
    try {
      if (storage_ == nullptr) {
        return false;
      }
      return storage_->remove(key);
    } catch (...) {
      return false;
    }
  }

  void clear() noexcept override {
    try {
      if (storage_ != nullptr) {
        storage_->clear();
      }
    } catch (...) {
      // Suppress all exceptions
    }
  }

 private:
  std::unique_ptr<PlatformStorage> storage_;
};

}  // namespace simple_tuner

#endif  // SIMPLE_TUNER_PLATFORM_COMMON_CONFIG_STORAGE_WRAPPER_H_
