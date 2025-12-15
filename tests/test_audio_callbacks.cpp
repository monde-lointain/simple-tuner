// Copyright 2025 SimpleTuner
#include <gtest/gtest.h>

#include <atomic>
#include <chrono>
#include <thread>
#include <vector>

#include "simple_tuner/interfaces/IAudioInput.h"
#include "simple_tuner/interfaces/IAudioOutput.h"
#include "simple_tuner/platform/desktop/MockAudioInput.h"
#include "simple_tuner/platform/desktop/MockAudioOutput.h"

namespace simple_tuner {
namespace {

constexpr double kSampleRate = 44100.0;
constexpr size_t kBufferSize = 512;
constexpr double kTestFrequency = 440.0;  // A4

class AudioCallbackTest : public ::testing::Test {
 protected:
  void SetUp() override {
    input_ = std::make_unique<MockAudioInput>();
    output_ = std::make_unique<MockAudioOutput>();

    ASSERT_TRUE(input_->initialize());
    ASSERT_TRUE(output_->initialize());

    // Verify sample rate matches expected
    EXPECT_DOUBLE_EQ(input_->get_sample_rate(), kSampleRate);
    EXPECT_DOUBLE_EQ(output_->get_sample_rate(), kSampleRate);
  }

  void TearDown() override {
    if (input_ && input_->is_active()) {
      input_->stop();
    }
    if (output_ && output_->is_active()) {
      output_->stop();
    }
  }

  std::unique_ptr<MockAudioInput> input_;
  std::unique_ptr<MockAudioOutput> output_;
};

TEST_F(AudioCallbackTest, InputCallbackExecutes) {
  std::atomic<int> callback_count{0};
  std::vector<float> captured_samples;

  // Simulate callback execution
  input_->set_test_frequency(kTestFrequency);
  ASSERT_TRUE(input_->start());

  // Read samples in chunks to simulate callback behavior
  for (int i = 0; i < 10; ++i) {
    std::vector<float> buffer(kBufferSize);
    size_t samples_read = input_->read_samples(buffer.data(), kBufferSize);

    if (samples_read > 0) {
      callback_count++;
      captured_samples.insert(captured_samples.end(), buffer.begin(),
                              buffer.begin() + samples_read);
    }

    // Simulate real-time callback timing (~11.6ms at 44.1kHz with 512 samples)
    std::this_thread::sleep_for(std::chrono::milliseconds(10));
  }

  EXPECT_GT(callback_count.load(), 0)
      << "Input callback should have executed at least once";
  EXPECT_GT(captured_samples.size(), 0u)
      << "Should have captured samples from input";
}

TEST_F(AudioCallbackTest, OutputCallbackExecutes) {
  std::atomic<int> callback_count{0};

  ASSERT_TRUE(output_->start());

  // Write samples in chunks to simulate callback behavior
  for (int i = 0; i < 10; ++i) {
    std::vector<float> buffer(kBufferSize);
    // Generate test pattern (simple ramp)
    for (size_t j = 0; j < kBufferSize; ++j) {
      buffer[j] = static_cast<float>(j) / kBufferSize;
    }

    size_t samples_written = output_->write_samples(buffer.data(), kBufferSize);

    if (samples_written > 0) {
      callback_count++;
    }

    std::this_thread::sleep_for(std::chrono::milliseconds(10));
  }

  EXPECT_GT(callback_count.load(), 0)
      << "Output callback should have executed at least once";

  // Verify MockAudioOutput captured samples
  const auto& captured = output_->get_captured_samples();
  EXPECT_GT(captured.size(), 0u)
      << "Output should have captured written samples";
}

TEST_F(AudioCallbackTest, InputOutputRoundTrip) {
  input_->set_test_frequency(kTestFrequency);

  ASSERT_TRUE(input_->start());
  ASSERT_TRUE(output_->start());

  std::vector<float> input_buffer(kBufferSize);
  size_t total_samples_processed = 0;

  // Simulate real-time processing loop
  for (int i = 0; i < 20; ++i) {
    // Read from input
    size_t samples_read = input_->read_samples(input_buffer.data(), kBufferSize);

    if (samples_read > 0) {
      // Process (pass-through in this test)
      // Write to output
      size_t samples_written =
          output_->write_samples(input_buffer.data(), samples_read);
      total_samples_processed += samples_written;
    }

    std::this_thread::sleep_for(std::chrono::milliseconds(10));
  }

  EXPECT_GT(total_samples_processed, 0u)
      << "Should have processed samples through input->output pipeline";

  const auto& captured = output_->get_captured_samples();
  EXPECT_EQ(captured.size(), total_samples_processed)
      << "Output should have captured all written samples";
}

TEST_F(AudioCallbackTest, CallbackFrequency) {
  input_->set_test_frequency(kTestFrequency);
  ASSERT_TRUE(input_->start());

  auto start_time = std::chrono::steady_clock::now();
  std::atomic<int> callback_count{0};
  const int target_callbacks = 50;

  // Run for a fixed duration
  while (callback_count.load() < target_callbacks) {
    std::vector<float> buffer(kBufferSize);
    size_t samples_read = input_->read_samples(buffer.data(), kBufferSize);

    if (samples_read > 0) {
      callback_count++;
    }

    // Expected callback interval: 512 samples / 44100 Hz = ~11.6ms
    std::this_thread::sleep_for(std::chrono::milliseconds(11));
  }

  auto end_time = std::chrono::steady_clock::now();
  auto duration_ms = std::chrono::duration_cast<std::chrono::milliseconds>(
                         end_time - start_time)
                         .count();

  // Expected duration: 50 callbacks × 11.6ms = ~580ms
  // Allow 50% tolerance for timing variability in tests
  EXPECT_GT(duration_ms, 400) << "Callbacks executing too fast";
  EXPECT_LT(duration_ms, 1000) << "Callbacks executing too slow";
}

TEST_F(AudioCallbackTest, StartStopSequence) {
  input_->set_test_frequency(kTestFrequency);

  // Start
  ASSERT_TRUE(input_->start());
  EXPECT_TRUE(input_->is_active());

  // Read samples
  std::vector<float> buffer(kBufferSize);
  size_t samples_read = input_->read_samples(buffer.data(), kBufferSize);
  EXPECT_GT(samples_read, 0u);

  // Stop
  input_->stop();
  EXPECT_FALSE(input_->is_active());

  // Restart
  ASSERT_TRUE(input_->start());
  EXPECT_TRUE(input_->is_active());

  samples_read = input_->read_samples(buffer.data(), kBufferSize);
  EXPECT_GT(samples_read, 0u);
}

TEST_F(AudioCallbackTest, MultipleBufferReads) {
  input_->set_test_frequency(kTestFrequency);
  ASSERT_TRUE(input_->start());

  constexpr int kNumReads = 100;
  std::vector<float> all_samples;

  for (int i = 0; i < kNumReads; ++i) {
    std::vector<float> buffer(kBufferSize);
    size_t samples_read = input_->read_samples(buffer.data(), kBufferSize);

    ASSERT_EQ(samples_read, kBufferSize)
        << "MockAudioInput should provide full buffers";

    all_samples.insert(all_samples.end(), buffer.begin(),
                       buffer.begin() + samples_read);
  }

  EXPECT_EQ(all_samples.size(), kNumReads * kBufferSize)
      << "Should have captured expected total sample count";

  // Verify signal continuity (no phase discontinuities)
  bool has_discontinuities = false;
  for (size_t i = 1; i < all_samples.size(); ++i) {
    float diff = std::abs(all_samples[i] - all_samples[i - 1]);
    if (diff > 0.5f) {  // Threshold for expected max change at 440 Hz
      has_discontinuities = true;
      break;
    }
  }
  EXPECT_FALSE(has_discontinuities)
      << "Signal should be continuous without phase jumps";
}

}  // namespace
}  // namespace simple_tuner
