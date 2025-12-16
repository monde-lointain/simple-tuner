#ifndef SIMPLE_TUNER_UI_TUNING_METER_COMPONENT_H_
#define SIMPLE_TUNER_UI_TUNING_METER_COMPONENT_H_

#include <juce_gui_basics/juce_gui_basics.h>

namespace simple_tuner {

// Week 1: Placeholder stub - full implementation in Week 6
class TuningMeterComponent : public juce::Component {
 public:
  TuningMeterComponent();
  ~TuningMeterComponent() override = default;

  // Update needle position (target position for damped animation)
  void update_needle_position(float cents) noexcept;

  // Show placeholder when no signal detected
  void set_no_signal() noexcept;

  // Component overrides
  void paint(juce::Graphics& g) override;
  void resized() override;

 private:
  float target_cents_;
  bool has_signal_;

  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(TuningMeterComponent)
};

}  // namespace simple_tuner

#endif  // SIMPLE_TUNER_UI_TUNING_METER_COMPONENT_H_
