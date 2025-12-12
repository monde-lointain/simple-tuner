#include "simple_tuner/platform/mobile/JUCEAudioInput.h"

#include "simple_tuner/platform/mobile/AudioManager.h"

namespace simple_tuner {

JUCEAudioInput::JUCEAudioInput() = default;

JUCEAudioInput::~JUCEAudioInput() { stop(); }

bool JUCEAudioInput::initialize() noexcept {
  try {
    auto& manager = AudioManager::get_instance();
    if (!manager.initialize()) {
      return false;
    }

    // Allocate circular buffer: platform_buffer_size × 32
    int buffer_size = manager.get_buffer_size();
    std::size_t circular_size = static_cast<std::size_t>(buffer_size * 32);
    circular_buffer_.resize(circular_size, 0.0f);

    return true;
  } catch (...) {
    return false;
  }
}

bool JUCEAudioInput::start() noexcept {
  try {
    auto& manager = AudioManager::get_instance();

    // Register input handler
    manager.set_input_handler([this](const float* data, int num_samples) {
      this->handle_input(data, num_samples);
    });

    if (!manager.start()) {
      return false;
    }

    active_.store(true);
    return true;
  } catch (...) {
    return false;
  }
}

void JUCEAudioInput::stop() noexcept {
  try {
    active_.store(false);
    auto& manager = AudioManager::get_instance();
    manager.set_input_handler(nullptr);
  } catch (...) {
    // Suppress exceptions
  }
}

std::size_t JUCEAudioInput::read_samples(float* buffer,
                                         std::size_t num_samples) noexcept {
  if (buffer == nullptr || num_samples == 0) {
    return 0;
  }

  try {
    std::lock_guard<std::mutex> lock(buffer_mutex_);

    std::size_t samples_read = 0;
    const std::size_t buffer_size = circular_buffer_.size();

    while (samples_read < num_samples && read_position_ != write_position_) {
      buffer[samples_read++] = circular_buffer_[read_position_];
      read_position_ = (read_position_ + 1) % buffer_size;
    }

    return samples_read;
  } catch (...) {
    return 0;
  }
}

double JUCEAudioInput::get_sample_rate() const noexcept {
  try {
    return AudioManager::get_instance().get_sample_rate();
  } catch (...) {
    return 44100.0;
  }
}

bool JUCEAudioInput::is_active() const noexcept { return active_.load(); }

void JUCEAudioInput::handle_input(const float* data, int num_samples) noexcept {
  if (data == nullptr || num_samples <= 0) {
    return;
  }

  try {
    std::lock_guard<std::mutex> lock(buffer_mutex_);

    const std::size_t buffer_size = circular_buffer_.size();

    for (int i = 0; i < num_samples; ++i) {
      circular_buffer_[write_position_] = data[i];
      write_position_ = (write_position_ + 1) % buffer_size;

      // Overwrite old data if buffer is full
      if (write_position_ == read_position_) {
        read_position_ = (read_position_ + 1) % buffer_size;
      }
    }
  } catch (...) {
    // Suppress exceptions in audio callback
  }
}

}  // namespace simple_tuner
