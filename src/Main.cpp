#include "simple_tuner/platform/mobile/AudioManager.h"
#include "simple_tuner/ui/MainComponent.h"

#include <juce_gui_basics/juce_gui_basics.h>

#if JUCE_IOS
#include "simple_tuner/platform/ios/iOSPermissions.h"
#elif JUCE_ANDROID
#include "simple_tuner/platform/android/AndroidPermissions.h"
#endif

class SimpleTunerApplication : public juce::JUCEApplication {
 public:
  SimpleTunerApplication() = default;

  const juce::String getApplicationName() override { return "SimpleTuner"; }
  const juce::String getApplicationVersion() override { return "0.1.0"; }
  bool moreThanOneInstanceAllowed() override { return false; }

  void initialise(const juce::String& command_line_parameters) override {
    (void)command_line_parameters;

    try {
      request_permissions();
    } catch (...) {
      DBG("Exception during permission request");
      quit();
    }
  }

  void shutdown() override {
    try {
      simple_tuner::AudioManager::get_instance().stop();
      main_window_ = nullptr;
    } catch (...) {
      DBG("Exception during shutdown");
    }
  }

  void anotherInstanceStarted(const juce::String& command_line) override {
    (void)command_line;
  }

  void systemRequestedQuit() override { quit(); }

  void unhandledException(const std::exception* e,
                          const juce::String& source_filename,
                          int line_number) override {
    (void)e;
    DBG("Unhandled exception at " << source_filename << ":" << line_number);
  }

 private:
  std::unique_ptr<juce::DocumentWindow> main_window_;

#if JUCE_IOS
  std::unique_ptr<simple_tuner::iOSPermissions> permissions_;
#elif JUCE_ANDROID
  std::unique_ptr<simple_tuner::AndroidPermissions> permissions_;
#endif

  void request_permissions() {
#if JUCE_IOS
    permissions_ = std::make_unique<simple_tuner::iOSPermissions>();

    auto status = permissions_->get_microphone_status();
    if (status == simple_tuner::PermissionStatus::kGranted) {
      initialize_audio_and_ui();
    } else if (status == simple_tuner::PermissionStatus::kNotDetermined) {
      permissions_->request_microphone_permission(
          [this](simple_tuner::PermissionStatus result) {
            handle_permission_result(result);
          });
    } else {
      show_permission_denied_message();
    }
#elif JUCE_ANDROID
    permissions_ = std::make_unique<simple_tuner::AndroidPermissions>();

    auto status = permissions_->get_microphone_status();
    if (status == simple_tuner::PermissionStatus::kGranted) {
      initialize_audio_and_ui();
    } else {
      permissions_->request_microphone_permission(
          [this](simple_tuner::PermissionStatus result) {
            handle_permission_result(result);
          });
    }
#else
    // Desktop or other platforms - no permissions needed
    initialize_audio_and_ui();
#endif
  }

  void handle_permission_result(simple_tuner::PermissionStatus status) {
    if (status == simple_tuner::PermissionStatus::kGranted) {
      initialize_audio_and_ui();
    } else {
      show_permission_denied_message();
    }
  }

  void initialize_audio_and_ui() {
    try {
      // Initialize audio
      auto& audio_manager = simple_tuner::AudioManager::get_instance();
      if (!audio_manager.initialize()) {
        DBG("Failed to initialize audio");
        show_audio_error_message();
        return;
      }

      if (!audio_manager.start()) {
        DBG("Failed to start audio");
        show_audio_error_message();
        return;
      }

      // Create main window
      main_window_ = std::make_unique<juce::DocumentWindow>(
          "SimpleTuner",
          juce::Desktop::getInstance().getDefaultLookAndFeel().findColour(
              juce::ResizableWindow::backgroundColourId),
          juce::DocumentWindow::allButtons);

      main_window_->setUsingNativeTitleBar(true);
      main_window_->setContentOwned(new simple_tuner::MainComponent(), true);

#if JUCE_IOS || JUCE_ANDROID
      main_window_->setFullScreen(true);
#else
      main_window_->centreWithSize(400, 600);
#endif

      main_window_->setVisible(true);
    } catch (...) {
      DBG("Exception during initialization");
      quit();
    }
  }

  void show_permission_denied_message() {
    juce::AlertWindow::showMessageBoxAsync(
        juce::AlertWindow::WarningIcon, "Permission Denied",
        "SimpleTuner needs microphone access to detect pitch. "
        "Please enable microphone permission in Settings.",
        "OK", nullptr,
        juce::ModalCallbackFunction::create([this](int) { quit(); }));
  }

  void show_audio_error_message() {
    juce::AlertWindow::showMessageBoxAsync(
        juce::AlertWindow::WarningIcon, "Audio Error",
        "Failed to initialize audio device. Please check your audio settings.",
        "OK", nullptr,
        juce::ModalCallbackFunction::create([this](int) { quit(); }));
  }

  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SimpleTunerApplication)
};

START_JUCE_APPLICATION(SimpleTunerApplication)
