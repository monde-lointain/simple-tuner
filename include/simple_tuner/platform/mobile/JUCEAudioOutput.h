#ifndef SIMPLE_TUNER_PLATFORM_MOBILE_JUCE_AUDIO_OUTPUT_H_
#define SIMPLE_TUNER_PLATFORM_MOBILE_JUCE_AUDIO_OUTPUT_H_

#include <atomic>
#include <cstddef>
#include <mutex>
#include <vector>

#include "simple_tuner/interfaces/IAudioOutput.h"

namespace simple_tuner {

class JUCEAudioOutput : public IAudioOutput {
 public:
  JUCEAudioOutput();
  ~JUCEAudioOutput() override;

  bool initialize() noexcept override;
  bool start() noexcept override;
  void stop() noexcept override;
  std::size_t write_samples(const float* buffer,
                            std::size_t num_samples) noexcept override;
  double get_sample_rate() const noexcept override;
  bool is_active() const noexcept override;

  JUCEAudioOutput(const JUCEAudioOutput&) = delete;
  JUCEAudioOutput& operator=(const JUCEAudioOutput&) = delete;
  JUCEAudioOutput(JUCEAudioOutput&&) = delete;
  JUCEAudioOutput& operator=(JUCEAudioOutput&&) = delete;

 private:
  void handle_output(float* data, int num_samples) noexcept;

  std::vector<float> circular_buffer_;
  std::size_t write_position_{0};
  std::size_t read_position_{0};
  std::mutex buffer_mutex_;
  std::atomic<bool> active_{false};
};

}  // namespace simple_tuner

#endif  // SIMPLE_TUNER_PLATFORM_MOBILE_JUCE_AUDIO_OUTPUT_H_
