#ifndef SIMPLE_TUNER_UI_UI_CONSTANTS_H_
#define SIMPLE_TUNER_UI_UI_CONSTANTS_H_

#include <juce_gui_basics/juce_gui_basics.h>

namespace simple_tuner {
namespace ui {

// Color Palette - Dark Mode Design
const juce::Colour kBackgroundDark = juce::Colour(0xFF1E1E1E);  // Charcoal
const juce::Colour kTextNeutral = juce::Colour(0xFFF0F0F0);     // Off-white
const juce::Colour kTextInTune = juce::Colour(0xFF4CAF50);  // Restrained green
const juce::Colour kTextInactive = juce::Colour(0xFF757575);  // Muted gray
const juce::Colour kMeterNeutral = juce::Colour(0xFF9E9E9E);  // Neutral gray

// Tuning Thresholds
constexpr float kInTuneThreshold = 5.0f;  // ±5 cents for "in tune" state

// Layout Ratios (percentage-based for responsive design)
constexpr float kNoteDisplayHeightRatio = 0.20f;  // 20% screen height
constexpr float kMeterHeightRatio = 0.65f;        // 65% screen height
constexpr float kControlsHeightRatio = 0.15f;     // 15% screen height
constexpr float kCentValueHeightRatio = 0.08f;    // 8% screen height

// Touch Target Sizing (iOS guidelines)
constexpr int kMinTouchTargetSize = 44;  // Minimum 44pt for accessibility

// Animation Parameters
constexpr float kDampingCoefficient = 0.80f;  // Needle damping: balanced

// Meter Geometry
constexpr float kMeterArcSpan = 150.0f;  // Arc span in degrees
constexpr float kMeterRadiusRatio =
    .60f;                                // Radius as % of width (90% diameter)
constexpr int kTickMarkInterval = 10;     // Tick marks every 10 cents
constexpr int kMeterRange = 20;           // ±20 cents display range
constexpr float kNeedleThickness = 2.0f;  // Needle line width

// Pure Interval Markers (cents deviation from equal temperament)
constexpr float kPureMajorThirdCents = -13.7f;  // Major 3rd pure intonation
constexpr float kPureMinorThirdCents = 15.6f;   // Minor 3rd pure intonation

// Typography
constexpr float kNoteLetterHeightRatio = 0.50f;       // 50% of component height
constexpr float kAccidentalHeightRatio = 0.35f;       // 35% of component height
constexpr float kReferenceFreqHeightRatio = 0.15f;    // 15% of component height
constexpr float kModeSelectorUnderlineHeight = 3.0f;  // Active mode underline

}  // namespace ui
}  // namespace simple_tuner

#endif  // SIMPLE_TUNER_UI_UI_CONSTANTS_H_
