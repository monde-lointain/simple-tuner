#include "simple_tuner/ui/ModeSelector.h"

#include "simple_tuner/ui/UIConstants.h"

namespace simple_tuner {

ModeSelector::ModeSelector() : current_mode_(Mode::kMeter) {}

void ModeSelector::set_mode(Mode mode) noexcept {
  if (current_mode_ != mode) {
    current_mode_ = mode;
    repaint();
  }
}

ModeSelector::Mode ModeSelector::get_mode() const noexcept {
  return current_mode_;
}

void ModeSelector::notify_mode_changed() noexcept {
  if (on_mode_changed) {
    on_mode_changed(current_mode_);
  }
}

void ModeSelector::paint(juce::Graphics& g) {
  // No background fill - let parent's background show through

  auto bounds = getLocalBounds();
  int segment_width = bounds.getWidth() / 2;

  // Draw segment labels
  g.setFont(juce::FontOptions(18.0f, juce::Font::bold));

  // Meter segment
  juce::Colour meter_color =
      (current_mode_ == Mode::kMeter) ? ui::kTextNeutral : ui::kTextInactive;
  g.setColour(meter_color);
  g.drawText("Meter", meter_bounds_, juce::Justification::centred);

  // Sound segment
  juce::Colour sound_color =
      (current_mode_ == Mode::kSound) ? ui::kTextNeutral : ui::kTextInactive;
  g.setColour(sound_color);
  g.drawText("Sound", sound_bounds_, juce::Justification::centred);

  // Draw active underline (3px green)
  g.setColour(ui::kTextInTune);
  int underline_y = bounds.getHeight() - ui::kModeSelectorUnderlineHeight;

  if (current_mode_ == Mode::kMeter) {
    g.fillRect(meter_bounds_.getX(), underline_y, segment_width,
               static_cast<int>(ui::kModeSelectorUnderlineHeight));
  } else {
    g.fillRect(sound_bounds_.getX(), underline_y, segment_width,
               static_cast<int>(ui::kModeSelectorUnderlineHeight));
  }
}

void ModeSelector::resized() {
  auto bounds = getLocalBounds();
  int segment_width = bounds.getWidth() / 2;

  meter_bounds_ = bounds.removeFromLeft(segment_width);
  sound_bounds_ = bounds;
}

void ModeSelector::mouseUp(const juce::MouseEvent& event) {
  auto pos = event.getPosition();

  if (meter_bounds_.contains(pos)) {
    if (current_mode_ != Mode::kMeter) {
      current_mode_ = Mode::kMeter;
      repaint();
      notify_mode_changed();
    }
  } else if (sound_bounds_.contains(pos)) {
    if (current_mode_ != Mode::kSound) {
      current_mode_ = Mode::kSound;
      repaint();
      notify_mode_changed();
    }
  }
}

}  // namespace simple_tuner
