#include "simple_tuner/platform/mobile/JUCEAudioOutput.h"

#include "simple_tuner/platform/mobile/AudioManager.h"

namespace simple_tuner {

JUCEAudioOutput::JUCEAudioOutput() = default;

JUCEAudioOutput::~JUCEAudioOutput() { stop(); }

bool JUCEAudioOutput::initialize() noexcept {
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

bool JUCEAudioOutput::start() noexcept {
  try {
    auto& manager = AudioManager::get_instance();

    // Register output handler
    manager.set_output_handler([this](float* data, int num_samples) {
      this->handle_output(data, num_samples);
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

void JUCEAudioOutput::stop() noexcept {
  try {
    active_.store(false);
    auto& manager = AudioManager::get_instance();
    manager.set_output_handler(nullptr);
  } catch (...) {
    // Suppress exceptions
  }
}

std::size_t JUCEAudioOutput::write_samples(const float* buffer,
                                           std::size_t num_samples) noexcept {
  if (buffer == nullptr || num_samples == 0) {
    return 0;
  }

  try {
    std::lock_guard<std::mutex> lock(buffer_mutex_);

    std::size_t samples_written = 0;
    const std::size_t buffer_size = circular_buffer_.size();

    for (std::size_t i = 0; i < num_samples; ++i) {
      std::size_t next_write = (write_position_ + 1) % buffer_size;

      // Check if buffer is full
      if (next_write == read_position_) {
        break;
      }

      circular_buffer_[write_position_] = buffer[i];
      write_position_ = next_write;
      samples_written++;
    }

    return samples_written;
  } catch (...) {
    return 0;
  }
}

double JUCEAudioOutput::get_sample_rate() const noexcept {
  try {
    return AudioManager::get_instance().get_sample_rate();
  } catch (...) {
    return 44100.0;
  }
}

bool JUCEAudioOutput::is_active() const noexcept { return active_.load(); }

void JUCEAudioOutput::handle_output(float* data, int num_samples) noexcept {
  if (data == nullptr || num_samples <= 0) {
    return;
  }

  try {
    std::lock_guard<std::mutex> lock(buffer_mutex_);

    const std::size_t buffer_size = circular_buffer_.size();

    for (int i = 0; i < num_samples; ++i) {
      if (read_position_ != write_position_) {
        data[i] = circular_buffer_[read_position_];
        read_position_ = (read_position_ + 1) % buffer_size;
      } else {
        // Buffer underrun - output silence
        data[i] = 0.0f;
      }
    }
  } catch (...) {
    // Suppress exceptions - output silence on error
    for (int i = 0; i < num_samples; ++i) {
      data[i] = 0.0f;
    }
  }
}

}  // namespace simple_tuner
