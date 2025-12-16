#include "simple_tuner/ui/StatusIndicatorComponent.h"

#include "simple_tuner/ui/UIConstants.h"

namespace simple_tuner {

StatusIndicatorComponent::StatusIndicatorComponent()
    : current_status_(Status::kNoSignal), status_text_("--") {}

void StatusIndicatorComponent::update_status(float cents) noexcept {
  if (cents < -ui::kInTuneThreshold) {
    current_status_ = Status::kFlat;
  } else if (cents > ui::kInTuneThreshold) {
    current_status_ = Status::kSharp;
  } else {
    current_status_ = Status::kInTune;
  }
  update_text();
  repaint();
}

void StatusIndicatorComponent::set_no_signal() noexcept {
  current_status_ = Status::kNoSignal;
  update_text();
  repaint();
}

void StatusIndicatorComponent::update_text() noexcept {
  switch (current_status_) {
    case Status::kFlat:
      status_text_ = "FLAT";
      break;
    case Status::kInTune:
      status_text_ = "IN TUNE";
      break;
    case Status::kSharp:
      status_text_ = "SHARP";
      break;
    case Status::kNoSignal:
      status_text_ = "--";
      break;
  }
}

void StatusIndicatorComponent::paint(juce::Graphics& g) {
  // No background fill - let parent's background show through

  // Text color: green for in tune, neutral otherwise
  juce::Colour text_color =
      (current_status_ == Status::kInTune) ? ui::kTextInTune : ui::kTextNeutral;

  g.setColour(text_color);
  g.setFont(juce::FontOptions(20.0f, juce::Font::bold));
  g.drawText(status_text_, getLocalBounds(), juce::Justification::centred);
}

void StatusIndicatorComponent::resized() {
  // No special layout needed - text is centered in paint()
}

}  // namespace simple_tuner
