#ifndef SIMPLE_TUNER_PLATFORM_MOBILE_AUDIO_MANAGER_H_
#define SIMPLE_TUNER_PLATFORM_MOBILE_AUDIO_MANAGER_H_

#include <atomic>
#include <functional>
#include <memory>

#include <juce_audio_devices/juce_audio_devices.h>

namespace simple_tuner {

class AudioManager : public juce::AudioIODeviceCallback {
 public:
  static AudioManager& get_instance() noexcept;

  bool initialize() noexcept;
  bool start() noexcept;
  void stop() noexcept;

  void set_input_handler(
      std::function<void(const float*, int)> handler) noexcept;
  void set_output_handler(std::function<void(float*, int)> handler) noexcept;

  double get_sample_rate() const noexcept;
  int get_buffer_size() const noexcept;

  void audioDeviceIOCallbackWithContext(
      const float* const* input_channel_data, int num_input_channels,
      float* const* output_channel_data, int num_output_channels,
      int num_samples,
      const juce::AudioIODeviceCallbackContext& context) override;

  void audioDeviceAboutToStart(juce::AudioIODevice* device) override;
  void audioDeviceStopped() override;

  AudioManager(const AudioManager&) = delete;
  AudioManager& operator=(const AudioManager&) = delete;
  AudioManager(AudioManager&&) = delete;
  AudioManager& operator=(AudioManager&&) = delete;

 private:
  AudioManager() = default;
  ~AudioManager() override = default;

  std::unique_ptr<juce::AudioDeviceManager> device_manager_;
  std::function<void(const float*, int)> input_handler_;
  std::function<void(float*, int)> output_handler_;
  std::atomic<double> sample_rate_{44100.0};
  std::atomic<int> buffer_size_{256};
};

}  // namespace simple_tuner

#endif  // SIMPLE_TUNER_PLATFORM_MOBILE_AUDIO_MANAGER_H_
