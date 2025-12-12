#include "simple_tuner/ui/MainComponent.h"

namespace simple_tuner {

MainComponent::MainComponent() {
  initialize_ui();
  setSize(400, 600);
}

MainComponent::~MainComponent() = default;

void MainComponent::initialize_ui() noexcept {
  try {
    // Mode label (Tuner/Tone Generator)
    mode_label_.setText("Tuner Mode", juce::dontSendNotification);
    mode_label_.setFont(juce::FontOptions(24.0f, juce::Font::bold));
    mode_label_.setJustificationType(juce::Justification::centred);
    addAndMakeVisible(mode_label_);

    // Note display
    note_label_.setText("--", juce::dontSendNotification);
    note_label_.setFont(juce::FontOptions(72.0f, juce::Font::bold));
    note_label_.setJustificationType(juce::Justification::centred);
    addAndMakeVisible(note_label_);

    // Frequency display
    frequency_label_.setText("0 Hz", juce::dontSendNotification);
    frequency_label_.setFont(juce::FontOptions(20.0f));
    frequency_label_.setJustificationType(juce::Justification::centred);
    addAndMakeVisible(frequency_label_);
  } catch (...) {
    DBG("Exception in MainComponent::initialize_ui()");
  }
}

void MainComponent::paint(juce::Graphics& g) {
  try {
    g.fillAll(juce::Colours::darkgrey);

    // Draw title bar background
    g.setColour(juce::Colours::black);
    g.fillRect(0, 0, getWidth(), 60);

    // Draw title text
    g.setColour(juce::Colours::white);
    g.setFont(juce::FontOptions(28.0f, juce::Font::bold));
    g.drawText("SimpleTuner", 0, 0, getWidth(), 60,
               juce::Justification::centred);
  } catch (...) {
    // Suppress paint exceptions
  }
}

void MainComponent::resized() {
  try {
    auto bounds = getLocalBounds();

    // Reserve top 60px for title bar
    bounds.removeFromTop(60);
    bounds.reduce(20, 20);

    // Mode label at top
    mode_label_.setBounds(bounds.removeFromTop(40));
    bounds.removeFromTop(40);  // Spacing

    // Note display in center
    note_label_.setBounds(bounds.removeFromTop(120));
    bounds.removeFromTop(20);  // Spacing

    // Frequency display below note
    frequency_label_.setBounds(bounds.removeFromTop(40));
  } catch (...) {
    DBG("Exception in MainComponent::resized()");
  }
}

}  // namespace simple_tuner
