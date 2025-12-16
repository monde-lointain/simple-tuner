#ifndef SIMPLE_TUNER_UI_MODE_SELECTOR_H_
#define SIMPLE_TUNER_UI_MODE_SELECTOR_H_

#include <functional>

#include <juce_gui_basics/juce_gui_basics.h>

namespace simple_tuner {

class ModeSelector : public juce::Component {
 public:
  enum class Mode { kMeter, kSound };

  ModeSelector();
  ~ModeSelector() override = default;

  // Mode management
  void set_mode(Mode mode) noexcept;
  Mode get_mode() const noexcept;

  // Callback for mode changes
  std::function<void(Mode)> on_mode_changed;

  // Component overrides
  void paint(juce::Graphics& g) override;
  void resized() override;
  void mouseUp(const juce::MouseEvent& event) override;

 private:
  Mode current_mode_;
  juce::Rectangle<int> meter_bounds_;
  juce::Rectangle<int> sound_bounds_;

  void notify_mode_changed() noexcept;

  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ModeSelector)
};

}  // namespace simple_tuner

#endif  // SIMPLE_TUNER_UI_MODE_SELECTOR_H_
