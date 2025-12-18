#ifndef SIMPLE_TUNER_UI_ALERT_HELPERS_H_
#define SIMPLE_TUNER_UI_ALERT_HELPERS_H_

#include <functional>

#include <juce_gui_basics/juce_gui_basics.h>

namespace simple_tuner {

// Displays a modal alert message box.
// If on_dismiss is provided, it will be called when the user dismisses the
// alert.
inline void show_alert(juce::AlertWindow::AlertIconType icon,
                       juce::String title, juce::String message,
                       juce::String button_text = "OK",
                       std::function<void()> on_dismiss = nullptr) {
  juce::ModalComponentManager::Callback* callback = nullptr;

  if (on_dismiss) {
    callback = juce::ModalCallbackFunction::create(
        [on_dismiss](int) { on_dismiss(); });
  }

  juce::AlertWindow::showMessageBoxAsync(icon, title, message, button_text,
                                         nullptr, callback);
}

}  // namespace simple_tuner

#endif  // SIMPLE_TUNER_UI_ALERT_HELPERS_H_
