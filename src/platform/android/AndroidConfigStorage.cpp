#include "simple_tuner/platform/android/AndroidConfigStorage.h"

#include <juce_data_structures/juce_data_structures.h>

namespace simple_tuner {

AndroidConfigStorage::AndroidConfigStorage() {
  try {
    juce::PropertiesFile::Options options;
    options.applicationName = kPrefsName;
    options.filenameSuffix = ".settings";
    options.osxLibrarySubFolder = "Application Support";
    options.folderName =
        juce::File::getSpecialLocation(juce::File::userApplicationDataDirectory)
            .getChildFile(kPrefsName)
            .getFullPathName();

    properties_ = std::make_unique<juce::PropertiesFile>(options);
  } catch (...) {
    // Leave properties_ null
  }
}

AndroidConfigStorage::~AndroidConfigStorage() = default;

bool AndroidConfigStorage::set_double(const std::string& key,
                                      double value) noexcept {
  try {
    if (properties_ == nullptr) {
      return false;
    }

    properties_->setValue(juce::String(key), value);
    return properties_->save();
  } catch (...) {
    return false;
  }
}

std::optional<double> AndroidConfigStorage::get_double(
    const std::string& key) const noexcept {
  try {
    if (properties_ == nullptr) {
      return std::nullopt;
    }

    juce::String juce_key(key);
    if (!properties_->containsKey(juce_key)) {
      return std::nullopt;
    }

    double value = properties_->getDoubleValue(juce_key);
    return value;
  } catch (...) {
    return std::nullopt;
  }
}

bool AndroidConfigStorage::remove(const std::string& key) noexcept {
  try {
    if (properties_ == nullptr) {
      return false;
    }

    properties_->removeValue(juce::String(key));
    return properties_->save();
  } catch (...) {
    return false;
  }
}

void AndroidConfigStorage::clear() noexcept {
  try {
    if (properties_ == nullptr) {
      return;
    }

    properties_->clear();
    properties_->save();
  } catch (...) {
    // Suppress exceptions
  }
}

}  // namespace simple_tuner
