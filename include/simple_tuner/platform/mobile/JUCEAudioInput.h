#ifndef SIMPLE_TUNER_PLATFORM_MOBILE_JUCE_AUDIO_INPUT_H_
#define SIMPLE_TUNER_PLATFORM_MOBILE_JUCE_AUDIO_INPUT_H_

#include <atomic>
#include <cstddef>
#include <mutex>
#include <vector>

#include "simple_tuner/interfaces/IAudioInput.h"

namespace simple_tuner {

class JUCEAudioInput : public IAudioInput {
 public:
  JUCEAudioInput();
  ~JUCEAudioInput() override;

  bool initialize() noexcept override;
  bool start() noexcept override;
  void stop() noexcept override;
  std::size_t read_samples(float* buffer,
                           std::size_t num_samples) noexcept override;
  double get_sample_rate() const noexcept override;
  bool is_active() const noexcept override;

  JUCEAudioInput(const JUCEAudioInput&) = delete;
  JUCEAudioInput& operator=(const JUCEAudioInput&) = delete;
  JUCEAudioInput(JUCEAudioInput&&) = delete;
  JUCEAudioInput& operator=(JUCEAudioInput&&) = delete;

 private:
  void handle_input(const float* data, int num_samples) noexcept;

  std::vector<float> circular_buffer_;
  std::size_t write_position_{0};
  std::size_t read_position_{0};
  std::mutex buffer_mutex_;
  std::atomic<bool> active_{false};
};

}  // namespace simple_tuner

#endif  // SIMPLE_TUNER_PLATFORM_MOBILE_JUCE_AUDIO_INPUT_H_
