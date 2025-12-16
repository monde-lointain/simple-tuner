#include "simple_tuner/ui/TuningMeterComponent.h"

#include <cmath>

#include "simple_tuner/ui/UIConstants.h"

namespace simple_tuner {

namespace {
constexpr float kPi = 3.14159265359f;
constexpr float kDegreesToRadians = kPi / 180.0f;
}  // namespace

TuningMeterComponent::TuningMeterComponent()
    : target_needle_position_(0.0f),
      current_needle_position_(0.0f),
      has_signal_(false),
      meter_center_x_(0.0f),
      meter_center_y_(0.0f),
      meter_radius_(0.0f) {
  startTimerHz(60);  // 60 FPS for smooth animation
}

TuningMeterComponent::~TuningMeterComponent() { stopTimer(); }

void TuningMeterComponent::update_needle_position(float cents) noexcept {
  target_needle_position_ = cents;
  has_signal_ = true;
}

void TuningMeterComponent::set_no_signal() noexcept {
  has_signal_ = false;
  target_needle_position_ = 0.0f;
}

void TuningMeterComponent::timerCallback() {
  // Apply damping to needle position
  current_needle_position_ +=
      (target_needle_position_ - current_needle_position_) *
      ui::kDampingCoefficient;
  repaint();
}

float TuningMeterComponent::cents_to_angle(float cents) const noexcept {
  // Map cents range [-100, +100] to 90° arc span
  // Center (0 cents) = 270° (pointing straight UP)
  // -100 cents (flat) = 315° (upper-left)
  // +100 cents (sharp) = 225° (upper-right)
  // Negative cents (flat) -> larger angle (LEFT)
  // Positive cents (sharp) -> smaller angle (RIGHT)

  constexpr float center_angle = 270.0f;  // Straight up
  constexpr float half_arc_span = 45.0f;  // 90° total / 2 = 45° each side
  constexpr float cents_range = 100.0f;   // ±100 cents

  // Normalize cents to [-1, 1] range
  float normalized = cents / cents_range;
  normalized = std::max(-1.0f, std::min(1.0f, normalized));

  // Map to angle: flat (negative) increases angle (goes left/counterclockwise)
  // sharp (positive) decreases angle (goes right/clockwise)
  // -100¢ = 345°, 0¢ = 270°, +100¢ = 195°
  float angle_degrees = center_angle + (normalized * half_arc_span);
  return angle_degrees * kDegreesToRadians;
}

void TuningMeterComponent::draw_arc(juce::Graphics& g) {
  // Arc drawing removed - dots and radial lines now extend to ±100 cents
  (void)g;  // Suppress unused parameter warning
}

void TuningMeterComponent::draw_radial_lines(juce::Graphics& g) {
  // Draw radial lines from center for every 5 cents (±100 range)
  // Lines extend from 1/3 to 95% of radius (bottom 1/3 cut off)
  constexpr float line_start_ratio = 0.333f;  // Start at 1/3 from center
  constexpr float line_end_ratio = 0.95f;  // End 95% of radius (short of dots)

  g.setColour(ui::kMeterNeutral.withAlpha(0.4f));  // Lighter gray

  for (int cents = -100; cents <= 100; cents += 5) {
    float angle = cents_to_angle(static_cast<float>(cents));

    float line_start_x =
        meter_center_x_ + (meter_radius_ * line_start_ratio) * std::cos(angle);
    float line_start_y =
        meter_center_y_ + (meter_radius_ * line_start_ratio) * std::sin(angle);

    float line_end_x =
        meter_center_x_ + (meter_radius_ * line_end_ratio) * std::cos(angle);
    float line_end_y =
        meter_center_y_ + (meter_radius_ * line_end_ratio) * std::sin(angle);

    g.drawLine(line_start_x, line_start_y, line_end_x, line_end_y, 1.0f);
  }
}

void TuningMeterComponent::draw_cent_dots(juce::Graphics& g) {
  // Draw dots at every 10 cents from -100 to +100
  // Dots at ±20, ±40, ±60, ±80, ±100 are 3x larger than others
  constexpr float small_dot_radius = 2.0f;
  constexpr float large_dot_radius = 6.0f;

  g.setColour(ui::kMeterNeutral);

  for (int cents = -100; cents <= 100; cents += 10) {
    float angle = cents_to_angle(static_cast<float>(cents));

    float dot_x = meter_center_x_ + meter_radius_ * std::cos(angle);
    float dot_y = meter_center_y_ + meter_radius_ * std::sin(angle);

    // Determine if this is a major interval (every 20 cents)
    bool is_major = (cents % 20 == 0 && cents != 0);
    float dot_radius = is_major ? large_dot_radius : small_dot_radius;

    g.fillEllipse(dot_x - dot_radius, dot_y - dot_radius, dot_radius * 2.0f,
                  dot_radius * 2.0f);
  }

  // Draw labels: "0" above center, "-20" and "+20" at their positions
  g.setFont(juce::FontOptions(12.0f, juce::Font::bold));
  g.setColour(ui::kMeterNeutral);

  // "0" label - positioned above the center dot (beyond the dot, on radial
  // line)
  float center_angle = cents_to_angle(0.0f);
  float center_label_distance = meter_radius_ + 25.0f;  // Beyond dot, outward
  float center_label_x =
      meter_center_x_ + center_label_distance * std::cos(center_angle);
  float center_label_y =
      meter_center_y_ + center_label_distance * std::sin(center_angle);
  g.drawText("0",
             juce::Rectangle<float>(center_label_x - 12.0f,
                                    center_label_y - 12.0f, 24.0f, 24.0f),
             juce::Justification::centred);

  // "-20" label - positioned at -20 cents, beyond the dot
  float minus_20_angle = cents_to_angle(-20.0f);
  float minus_20_distance = meter_radius_ + 25.0f;
  float minus_20_x =
      meter_center_x_ + minus_20_distance * std::cos(minus_20_angle);
  float minus_20_y =
      meter_center_y_ + minus_20_distance * std::sin(minus_20_angle);
  g.drawText("-20",
             juce::Rectangle<float>(minus_20_x - 16.0f, minus_20_y - 12.0f,
                                    32.0f, 24.0f),
             juce::Justification::centred);

  // "+20" label - positioned at +20 cents, beyond the dot
  float plus_20_angle = cents_to_angle(20.0f);
  float plus_20_distance = meter_radius_ + 25.0f;
  float plus_20_x =
      meter_center_x_ + plus_20_distance * std::cos(plus_20_angle);
  float plus_20_y =
      meter_center_y_ + plus_20_distance * std::sin(plus_20_angle);
  g.drawText("+20",
             juce::Rectangle<float>(plus_20_x - 16.0f, plus_20_y - 12.0f, 32.0f,
                                    24.0f),
             juce::Justification::centred);
}

void TuningMeterComponent::draw_interval_marker(juce::Graphics& g,
                                                float cents) {
  // These are optional - not currently drawn, reserved for future use
  // Pure interval markers could be drawn here if desired
}

void TuningMeterComponent::draw_needle(juce::Graphics& g) {
  if (!has_signal_) {
    return;
  }
  constexpr float line_end_ratio = 0.95f;  // End 95% of radius (short of dots)

  float needle_angle = cents_to_angle(current_needle_position_);

  // Needle extends from center to arc
  float needle_end_x = meter_center_x_ + (meter_radius_ * line_end_ratio) *
                                             std::cos(needle_angle);
  float needle_end_y = meter_center_y_ + (meter_radius_ * line_end_ratio) *
                                             std::sin(needle_angle);

  // Determine needle color based on in-tune status
  juce::Colour needle_color =
      (std::abs(current_needle_position_) <= ui::kInTuneThreshold)
          ? ui::kTextInTune
          : ui::kMeterNeutral;

  g.setColour(needle_color);

  // Draw main needle line
  g.drawLine(meter_center_x_, meter_center_y_, needle_end_x, needle_end_y,
             ui::kNeedleThickness);
}

void TuningMeterComponent::paint(juce::Graphics& g) {
  // Draw in order from back to front:
  draw_arc(g);           // Arc outline
  draw_radial_lines(g);  // Radial lines radiating from center
  draw_cent_dots(g);     // Dots at 10-cent intervals
  draw_interval_marker(g, ui::kPureMajorThirdCents);  // Reserved for future
  draw_interval_marker(g, ui::kPureMinorThirdCents);  // Reserved for future
  draw_needle(g);                                     // Needle on top
}

void TuningMeterComponent::resized() {
  auto bounds = getLocalBounds();

  // Calculate meter geometry
  meter_radius_ = bounds.getWidth() * ui::kMeterRadiusRatio;
  meter_center_x_ = bounds.getCentreX();

  // Position center at 85% down for visual balance
  meter_center_y_ = bounds.getY() + bounds.getHeight() * 0.85f;

  // Precompute tick angles (optional optimization)
  tick_angles_.clear();
  for (int cents = -ui::kMeterRange; cents <= ui::kMeterRange;
       cents += ui::kTickMarkInterval) {
    tick_angles_.push_back(cents_to_angle(static_cast<float>(cents)));
  }
}

}  // namespace simple_tuner
