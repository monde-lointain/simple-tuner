#include "simple_tuner/ui/TuningMeterComponent.h"

#include "simple_tuner/ui/UIConstants.h"

namespace simple_tuner {

TuningMeterComponent::TuningMeterComponent()
    : target_cents_(0.0f), has_signal_(false) {}

void TuningMeterComponent::update_needle_position(float cents) noexcept {
  target_cents_ = cents;
  has_signal_ = true;
  repaint();
}

void TuningMeterComponent::set_no_signal() noexcept {
  has_signal_ = false;
  target_cents_ = 0.0f;
  repaint();
}

void TuningMeterComponent::paint(juce::Graphics& g) {
  // Week 1 placeholder: draw a subtle outline to show meter area
  g.setColour(ui::kMeterNeutral.withAlpha(0.3f));
  g.drawRect(getLocalBounds().toFloat(), 2.0f);

  // Draw placeholder text
  g.setColour(ui::kTextInactive);
  g.setFont(juce::FontOptions(16.0f));
  g.drawText("Tuning Meter (Week 6)", getLocalBounds(),
             juce::Justification::centred);

  // Week 6: Full arc + needle + tick marks implementation
}

void TuningMeterComponent::resized() {
  // Week 6: Calculate arc geometry
}

}  // namespace simple_tuner
