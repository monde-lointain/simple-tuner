#ifndef SIMPLE_TUNER_UI_NOTE_DISPLAY_COMPONENT_H_
#define SIMPLE_TUNER_UI_NOTE_DISPLAY_COMPONENT_H_

#include <memory>

#include <juce_gui_basics/juce_gui_basics.h>

namespace simple_tuner {

class FrequencyCalculator;

class NoteDisplayComponent : public juce::Component {
 public:
  explicit NoteDisplayComponent(std::shared_ptr<FrequencyCalculator> freq_calc);
  ~NoteDisplayComponent() override = default;

  // Update displayed note from MIDI number
  void update_note(int midi_note, double confidence) noexcept;

  // Update displayed note with cents deviation for color
  void update_note_with_cents(int midi_note, double confidence,
                              float cents) noexcept;

  // Show placeholder when no signal detected
  void set_no_signal() noexcept;

  // Component overrides
  void paint(juce::Graphics& g) override;
  void resized() override;

 private:
  std::shared_ptr<FrequencyCalculator> frequency_calculator_;

  juce::String note_letter_;
  juce::String accidental_;
  juce::String reference_freq_;
  int current_midi_note_;
  float current_cents_;
  bool has_signal_;

  void update_display_from_midi(int midi_note) noexcept;

  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(NoteDisplayComponent)
};

}  // namespace simple_tuner

#endif  // SIMPLE_TUNER_UI_NOTE_DISPLAY_COMPONENT_H_
