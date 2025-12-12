#ifndef SIMPLE_TUNER_UI_MAIN_COMPONENT_H_
#define SIMPLE_TUNER_UI_MAIN_COMPONENT_H_

#include <juce_gui_basics/juce_gui_basics.h>

namespace simple_tuner {

class MainComponent : public juce::Component {
 public:
  MainComponent();
  ~MainComponent() override;

  void paint(juce::Graphics& g) override;
  void resized() override;

 private:
  juce::Label mode_label_;
  juce::Label note_label_;
  juce::Label frequency_label_;

  void initialize_ui() noexcept;

  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MainComponent)
};

}  // namespace simple_tuner

#endif  // SIMPLE_TUNER_UI_MAIN_COMPONENT_H_
