#ifndef SIMPLE_TUNER_UI_TUNING_METER_COMPONENT_H_
#define SIMPLE_TUNER_UI_TUNING_METER_COMPONENT_H_

#include <vector>

#include <juce_gui_basics/juce_gui_basics.h>

namespace simple_tuner {

// Week 2: Full tuning meter with arc, tick marks, and animated needle
class TuningMeterComponent : public juce::Component, private juce::Timer {
 public:
  TuningMeterComponent();
  ~TuningMeterComponent() override;

  // Update needle position (target position for damped animation)
  void update_needle_position(float cents) noexcept;

  // Show placeholder when no signal detected
  void set_no_signal() noexcept;

  // Component overrides
  void paint(juce::Graphics& g) override;
  void resized() override;

 private:
  // Needle state
  float target_needle_position_;
  float current_needle_position_;
  bool has_signal_;

  // Arc geometry (calculated in resized())
  float meter_center_x_;
  float meter_center_y_;
  float meter_radius_;
  std::vector<float> tick_angles_;

  // Timer for needle animation
  void timerCallback() override;

  // Helper methods
  float cents_to_angle(float cents) const noexcept;
  void draw_arc(juce::Graphics& g);
  void draw_radial_lines(juce::Graphics& g);
  void draw_cent_dots(juce::Graphics& g);
  void draw_interval_marker(juce::Graphics& g, float cents);
  void draw_needle(juce::Graphics& g);

  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(TuningMeterComponent)
};

}  // namespace simple_tuner

#endif  // SIMPLE_TUNER_UI_TUNING_METER_COMPONENT_H_
