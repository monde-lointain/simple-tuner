# SimpleTuner - Implementation Plan

## Executive Summary

This implementation plan provides a phased approach to building SimpleTuner using the JUCE framework. The plan follows an incremental development strategy, building and testing core functionality before adding features. Each phase includes specific implementation steps, deliverables, success criteria, and traceability to functional requirements and business rules.

**Total Duration:** 12 weeks
**Target Platforms:** iOS 13+, Android 8.0+, Desktop (test environment)
**Framework:** JUCE 7.0+ (mobile platforms), CMake (desktop testing)
**Build Systems:** CMake (desktop/CI), Projucer (mobile platforms)

---

## Phase 1: Desktop Testing Infrastructure and Project Foundation (Weeks 1-2)

### Objectives
- **Week 1:** Establish desktop-first testing infrastructure
- **Week 2:** Implement mobile platform project structure and audio infrastructure
- Prioritize rapid testing capability over mobile deployment
- Create foundation for CI/CD automation

### Week 1: Desktop Testing Setup

**Implementation Steps:**

1. **Desktop CMake Project Setup**
   - Create CMakeLists.txt in project root
   - Define shared library target (`simple_tuner_core`) containing platform-independent code
   - Define desktop test executable target (`simple_tuner_tests`)
   - Configure C++17 standard, enable warnings (-Wall -Wextra)
   - Set up directory structure:
     ```
     include/          # Public headers for simple_tuner_core
       simple_tuner/
         algorithms/   # Public algorithm headers
         interfaces/   # Public abstract interfaces
         config/       # Public configuration headers
     src/
       shared/         # Platform-independent core (≥95% of algorithm code)
         algorithms/   # Pitch detection, tone generation, frequency calc
         interfaces/   # Abstract interfaces for platform services
         config/       # Configuration management
       platform/
         desktop/      # Mock implementations for testing
         ios/          # iOS-specific code (Week 2)
         android/      # Android-specific code (Week 2)
     tests/            # Unit tests for shared library
     ```

2. **Platform Abstraction Layer Design**
   - Define `IAudioInput` interface (abstract microphone/test signal injection)
   - Define `IAudioOutput` interface (abstract speakers/test verification)
   - Define `IConfigStorage` interface (abstract UserDefaults/SharedPreferences/mock)
   - Define `IPermissions` interface (abstract permission handling/auto-grant)
   - Document interface contracts with clear comments
   - Verify interfaces contain no platform-specific types (pure abstract C++)

3. **Mock Platform Implementations (Desktop)**
   - Create `MockAudioInput`: Injects synthetic sine waves at specified frequencies
   - Create `MockAudioOutput`: Captures generated samples for verification
   - Create `MockConfigStorage`: In-memory key-value store for test isolation
   - Create `MockPermissions`: Auto-grants all permissions for test environment
   - Implement minimal functionality sufficient for unit testing

4. **Unit Test Framework Integration**
   - Add Google Test as dependency (via CMake FetchContent)
   - Create first test file: `tests/test_frequency_calculator.cpp`
   - Implement basic test: Verify A4 = 440 Hz calculation
   - Configure CMake to build and execute tests: `make test` or `ctest`
   - Verify test execution and reporting

5. **Shared Library Stub Implementation**
   - Create `FrequencyCalculator` class skeleton (equal temperament formula)
   - Create `PitchDetector` class skeleton (placeholder implementation)
   - Create `ToneGenerator` class skeleton (placeholder sine synthesis)
   - Create `ConfigManager` class skeleton (reference pitch logic)
   - Implement only frequency calculation fully in Week 1 (rest in later phases)
   - Ensure all classes compile and link in shared library

6. **Desktop Build Verification**
   - Build desktop test executable via CMake
   - Execute test suite locally (should have ≥1 passing test)
   - Verify test output format (TAP or JUnit XML for CI integration)
   - Measure test execution time (should be <5 seconds at this stage)
   - Document build instructions in README.md

7. **CI/CD Pipeline Setup**
   - Create CI configuration file (e.g., `.github/workflows/ci.yml` for GitHub Actions)
   - Configure Stage 1: Build desktop executable + run tests
   - Configure failure notifications (email, Slack, etc.)
   - Verify CI executes on commit to feature branch
   - Document CI setup and requirements

**Deliverables (Week 1):**
- CMake build system compiling shared library and test executable
- Platform abstraction interfaces defined
- Mock implementations for desktop testing
- ≥1 unit test passing (frequency calculation)
- CI/CD pipeline running desktop tests
- Documentation: Build instructions, architecture overview

**Success Criteria (Week 1):**
- ✓ Desktop test executable builds
- ✓ Unit test framework executes and reports results
- ✓ CI pipeline triggers on commit and reports status
- ✓ Shared library code compiles without platform-specific dependencies
- ✓ Mock implementations enable test execution without real audio hardware

---

### Week 2: Mobile Platform Setup

**Implementation Steps:**

1. **Projucer Mobile Project Setup**
   - Create JUCE Projucer project with iOS and Android exporters
   - Configure project settings (bundle IDs, version, app name)
   - Set minimum OS versions (iOS 13.0, Android API 26)
   - Enable required JUCE modules (audio_basics, audio_devices, dsp, gui_basics, core)
   - Configure C++17 standard in project settings
   - **Link against shared library:** Configure Projucer to compile `src/shared/` code

2. **Mobile Build System Configuration**
   - Generate Xcode project for iOS development
   - Generate Android Studio project with NDK configuration
   - Configure code signing for iOS (development certificates)
   - Configure Android keystore for signing
   - Verify clean builds on both platforms
   - **Ensure shared library code compiles identically** in mobile builds

3. **Mobile Platform Implementations**
   - Create `JUCEAudioInput` implementing `IAudioInput` (wraps AudioDeviceManager)
   - Create `JUCEAudioOutput` implementing `IAudioOutput` (wraps AudioDeviceManager)
   - Create `iOSConfigStorage` implementing `IConfigStorage` (wraps UserDefaults)
   - Create `AndroidConfigStorage` implementing `IConfigStorage` (wraps SharedPreferences)
   - Create `iOSPermissions` implementing `IPermissions` (wraps AVAudioSession)
   - Create `AndroidPermissions` implementing `IPermissions` (wraps ActivityCompat)

4. **Audio Device Initialization (Mobile)**
   - Create `AudioManager` class wrapping JUCE AudioDeviceManager
   - Implement audio device initialization with error handling
   - Configure mono input (1 channel) and stereo output (2 channels)
   - Request minimum buffer size for low latency
   - Implement sample rate detection (44.1/48 kHz)

5. **Audio Callback Implementation (Mobile)**
   - Create `AudioProcessor` class implementing AudioIODeviceCallback
   - Implement processBlock() for real-time audio handling
   - Add input buffer access for pitch detection (placeholder)
   - Add output buffer synthesis for tone generation (placeholder)
   - Verify callback thread priority (real-time)

6. **Basic UI Framework (Mobile)**
   - Create `MainComponent` extending JUCE Component
   - Implement basic window with title bar
   - Add placeholder UI elements (labels for mode, note display)
   - Implement paint() method for basic rendering
   - Test UI responsiveness to resize events

7. **Platform Permission Handling (Mobile)**
   - Add microphone permission strings to Info.plist (iOS)
   - Add RECORD_AUDIO permission to AndroidManifest.xml
   - Implement permission request flow on first launch
   - Handle permission granted/denied states
   - Test permission persistence across app restarts

8. **Mobile Build Verification**
   - Build iOS project in Xcode
   - Build Android project in Android Studio
   - Deploy to test devices (iPhone, Android phone)
   - Verify application launches without crashes
   - Verify microphone permission dialog appears
   - Verify audio callback executes (log statements)

9. **CI/CD Mobile Build Integration**
   - Add Stage 2 to CI pipeline: iOS compilation verification
   - Add Stage 3 to CI pipeline: Android compilation verification
   - Configure mobile stages to only run if Stage 1 (desktop tests) passes
   - Verify gating: Failed desktop tests prevent mobile builds

**Deliverables (Week 2):**
- Compiling iOS and Android projects
- Basic application launching on test devices
- Audio callback executing without glitches
- Microphone permission flow functional
- Mobile platform implementations of abstraction interfaces
- CI/CD pipeline with 3 stages (desktop tests → iOS build → Android build)

**Success Criteria (Week 2):**
- ✓ Clean builds on macOS (Xcode) and Android Studio
- ✓ Application launches without crashes on iOS and Android
- ✓ Audio callback executes at 44.1/48 kHz without underruns
- ✓ Microphone permission dialog appears and handles responses
- ✓ Shared library code compiles identically in desktop and mobile builds
- ✓ CI pipeline gates mobile builds on desktop test success
- ✓ No memory leaks detected in basic operation (profiling tools)

---

### Requirements Implemented (Phase 1 Overall)

**Functional Requirements:**
- FR-083: Microphone Permission Request
- FR-084: Permission Status Detection
- FR-086: Audio Device Initialization
- FR-087: Sample Rate Adaptation
- FR-089: Audio Callback Implementation
- FR-053: Frequency Recalculation (infrastructure)
- FR-054: Persistent Storage

**Non-Functional Requirements:**
- **PO-011:** Shared Library Architecture
- **PO-012:** Platform Abstraction Interfaces
- **PO-013:** Desktop Test Runner
- **MA-001:** Modularity and Separation of Concerns
- **MA-002:** Testability
- **MA-004:** Build System Maintainability

**Business Rules:**
- BR-018: Default reference pitch is 440 Hz on first launch
- BR-020: Reference pitch must persist across application launches

---

## Phase 2: Pitch Detection Algorithm (Weeks 3-4)

### Objectives
- Implement McLeod Pitch Period Method
- Add frequency-to-note mapping
- Calculate cent deviation
- Test detection accuracy

### Implementation Steps

**Desktop Testing (Continuous):**
- Write unit tests for all new algorithmic code implemented in this phase
- Execute tests locally before committing
- Verify CI pipeline passes before proceeding to mobile device testing
- Target: Maintain ≥80% code coverage of shared library
- All new code in `src/shared/` must have corresponding tests in `tests/`

**Mobile Implementation:**

1. **Autocorrelation Infrastructure**
   - Create `PitchDetector` class for analysis
   - Implement autocorrelation computation for time-domain analysis
   - Allocate buffers for autocorrelation and NSDF calculations
   - Implement NSDF (Normalized Square Difference Function)
   - Test autocorrelation with known test signals

2. **Audio Buffering**
   - Implement circular buffer to accumulate samples for analysis
   - Handle buffer filling across multiple audio callbacks
   - Trigger analysis when sufficient samples available
   - Optimize buffer management for minimal latency

3. **Fundamental Frequency Detection**
   - Compute autocorrelation function over audio buffer
   - Calculate NSDF (Normalized Square Difference Function)
   - Implement peak detection algorithm in NSDF
   - Identify highest-clarity peak corresponding to fundamental period
   - Apply parabolic interpolation for sub-sample period accuracy
   - Convert period to frequency (f = sample_rate / period)

4. **Signal Validation**
   - Implement signal level detection (peak amplitude measurement)
   - Add threshold check (-40 dBFS minimum)
   - Calculate detection confidence metric
   - Reject ambiguous detections (multiple competing peaks)
   - Handle "no signal" state

5. **Note Identification**
   - Create `FrequencyCalculator` class for conversions
   - Implement frequency-to-MIDI-note-number conversion
   - Calculate: `note_number = round(12 × log₂(f / f_ref))`
   - Map MIDI number to note name (C, C#, D, etc.)
   - Determine octave number from MIDI note

6. **Cent Deviation Calculation**
   - Calculate target frequency for detected note
   - Compute cent deviation: `cents = 1200 × log₂(f_detected / f_target)`
   - Clamp display range to ±50 cents
   - Round to 0.1 cent precision for display

7. **Note Persistence Logic**
   - Implement 100ms stability timer before note change
   - Track consecutive detections of same note
   - Update display only after confirmation period
   - Prevent rapid note flickering

8. **Testing and Calibration**
   - Create test signal generator for known frequencies
   - Test detection across frequency range (C1-C8)
   - Measure accuracy with calibrated tones
   - Optimize autocorrelation window size for accuracy vs latency
   - Profile CPU usage of detection algorithm

### Deliverables
- PitchDetector class with complete analysis pipeline
- FrequencyCalculator utility class
- Unit tests for frequency calculations
- Accuracy validation report (±1 cent target)
- CPU usage measurements

### Success Criteria
- ✓ Detection accuracy within ±1 cent for 40+ dB SNR signals
- ✓ Frequency range coverage: C1 (32.70 Hz) to C8 (4186.01 Hz)
- ✓ CPU usage ≤15% on minimum-spec devices during detection
- ✓ Detection latency ≤30 ms from audio input to result
- ✓ Harmonic rejection: fundamentals correctly identified
- ✓ No false detections when no signal present
- ✓ Unit tests pass for all frequency calculations

### Requirements Implemented
**Functional Requirements:**
- FR-001: Continuous Audio Monitoring
- FR-002: Frequency Range Coverage
- FR-003: Fundamental Frequency Extraction
- FR-004: Noise Rejection
- FR-005: Chromatic Note Identification
- FR-006: Octave Number Assignment
- FR-007: Cent Deviation Calculation
- FR-008: Detection Accuracy
- FR-017: Detection Confidence Handling
- FR-018: Pitch Detection Latency
- FR-020: Stable Note Persistence
- FR-100: Sample Rate Conversion Prevention

**Business Rules:**
- BR-001: Detection range must span C1 to C8
- BR-002: Detection accuracy within ±1 cent for signals ≥40 dB SNR
- BR-003: Pitch detection latency must not exceed 30ms
- BR-006: Note persistence requires 100ms stability
- BR-007: All frequencies calculated using 12-tone equal temperament
- BR-013: Detection frequency range spans C1 to C8

---

## Phase 3: Meter Mode UI Implementation (Weeks 5-6)

### Objectives
- Create visual tuning meter display
- Implement note name and cent value displays
- Add status indicators (flat/center/sharp)
- Connect UI to pitch detection
- Implement design philosophy: **measurement instrument on glass**

### Design Philosophy Integration
All UI elements must embody the core design principle: a professional measurement instrument displayed on the phone screen with no physical enclosure representation. Interaction is deliberate and minimal; feedback is informational, never expressive. The dark background and restrained visual treatment should convey calm professionalism, encouraging immediate user trust without need for exploration.

### Implementation Steps

**Desktop Testing (Continuous):**
- Write unit tests for all new algorithmic code implemented in this phase
- Execute tests locally before committing
- Verify CI pipeline passes before proceeding to mobile device testing
- Target: Maintain ≥80% code coverage of shared library
- All new code in `src/shared/` must have corresponding tests in `tests/`

**Mobile Implementation:**

1. **Layout Structure and Visual Foundation**
   - Full-screen dark background: charcoal or near-black (not pure #000000), matte appearance
   - No visible texture, noise, or strong gradients
   - Divide screen into three zones (landscape primary, portrait secondary):
     - **Top Zone:** Note name and status information
     - **Center Zone:** Tuning meter (primary visual anchor)
     - **Bottom Zone:** Controls and mode selector
   - No scrolling; all core information visible at all times
   - Create `MeterModeComponent` extending JUCE Component

2. **Note Name Display (Top Zone)**
   - Create `NoteDisplayComponent` for centered note name
   - **Visual treatment:** Large, bold, sans-serif font (modern, technical character)
   - **Font hierarchy:** Single letter dominant (e.g., A), accidentals (♯/♭) smaller and cleanly aligned
   - **Dynamic sizing:** ≥15% of screen height for large visibility
   - **Color behavior:**
     - Neutral off-white when unstable or no signal
     - Soft green when stably in tune (±5 cents threshold)
   - **Supporting information below note:**
     - Reference frequency (e.g., "440 Hz")
     - Smaller text, low visual emphasis
   - Handle "no signal" state with placeholder ("—" in neutral color)

3. **Tuning Meter Visualization (Center Zone)**
   - Create `TuningMeterComponent` as the primary visual anchor
   - **Meter design:**
     - Wide arc or semi-circular meter spanning most screen width
     - Arc angle: 120-180 degrees (aesthetic balance)
     - Thin, precise tick marks at 10-cent intervals
     - Center point clearly labeled as "0"
     - Scale represents ±20 cents deviation (clamped display range)
   - **Needle/Indicator:**
     - Thin vertical line or narrow wedge shape
     - Smooth, damped motion (coefficient 0.7-0.85)
     - No bouncing or spring effects
     - Updated continuously from pitch detection
   - **Color behavior:**
     - Neutral gray or off-white when away from center
     - Subtle transition to restrained green near perfect tuning
     - No flashing, pulsing, or attention-seeking animation
   - **Colored zones:**
     - Red/muted zone: far from center (flat or sharp)
     - Green zone: ±5 cents (in tune)
     - Zones integrated into meter arc, not overlaid
   - **Pure interval reference marks:**
     - Marker at -13.7 cents (pure major third)
     - Marker at +15.6 cents (pure minor third)
     - Distinct visual style (small triangle or dot)
     - Visible but subtle; do not compete with main needle

4. **Cent Value Numeric Display**
   - Create adjacent to or below tuning meter
   - **Format:** sign + value to 1 decimal place + "cents" (e.g., "-12.3 cents")
   - **Font size:** ≥8% of screen height, clean sans-serif
   - **Color:** Off-white (neutral); soft green when |cents| ≤ 5 (in-tune zone)
   - Update synchronized with meter needle

5. **Status Indicators**
   - Create text labels: "FLAT", "IN TUNE", "SHARP"
   - Position below or integrated with meter zones
   - Highlight active status with boldness/emphasis
   - Update based on cent deviation thresholds (±5 cents = "IN TUNE")
   - Color aligned with meter zone colors

6. **Mode Selector Control (Bottom Zone)**
   - Create segmented control or tab-like interface: "Meter" | "Sound"
   - Position at bottom or top of screen, consistent layout in both orientations
   - Highlight active mode with contrast
   - Handle tap events to switch modes
   - **Interaction:** Tap-based only, no gestures
   - **Visual treatment:** Flat, minimal iconography; same visual weight as other controls
   - Minimum touch target size: 44×44 pt (iOS) / 48×48 dp (Android)
   - Controls should feel like **instrument switches**, not app buttons

7. **Settings Button**
   - Add subtle label or gear icon button (top-right corner)
   - Minimum size: 44×44 pt / 48×48 dp
   - Flat, minimal icon style (no fill, outline only preferred)
   - Same visual weight as mode selector
   - Handle tap to navigate to settings screen
   - Use platform-appropriate icon, but maintain consistency with design language

8. **UI Update Mechanism**
   - Implement thread-safe communication from audio to UI thread
   - Use JUCE MessageManager for asynchronous updates
   - Target ≥20 Hz display refresh rate (50ms intervals)
   - Smooth meter needle animation using interpolation
   - Prevent UI updates from blocking audio thread
   - No jank or stuttering during rapid updates

9. **Visual Design Summary**
   - **Color Palette:**
     - Background: dark charcoal / near-black
     - Primary text: off-white
     - In-tune indication: restrained green (subtle, not neon)
     - Inactive elements: muted gray
   - **Typography:**
     - Clean, modern sans-serif (neutral, technical character)
     - Heavy weight: Note name
     - Medium weight: Labels and status indicators
     - Light weight: Reference frequency, secondary info
   - **Contrast:** Ensure WCAG AA minimum (4.5:1) for readability
   - **Motion:** Only used to convey measurement; physically believable meter movement
   - **Feedback:** Subtle visual highlight on tap (opacity change, not ripple/splash effects)
   - **No decoration:** No gradients, shadows, rounded corners, or expressive styling

10. **Safe Area Handling**
    - Query safe area insets from platform
    - Adjust layout to avoid notches, rounded corners, home indicators
    - Ensure controls not obscured by system UI
    - Test on notched devices (iPhone X+) and hole-punch cameras

### Deliverables
- Complete Meter Mode UI components
- Animated tuning meter with needle
- Note and cent value displays
- Status indicators
- Mode selector control
- Settings navigation

### Success Criteria
- ✓ Note name visible and legible from 1 meter distance
- ✓ Meter updates at ≥20 Hz (50ms intervals)
- ✓ No UI jank or stuttering during updates
- ✓ Orientation changes complete within 200ms
- ✓ All touch targets meet platform size guidelines
- ✓ Visual design meets contrast requirements
- ✓ Layout adapts to screen sizes 4.7" to 6.7"

### Requirements Implemented
**Functional Requirements:**
- FR-009: Note Name Display
- FR-010: Tuning Meter Visualization
- FR-011: Flat Indicator
- FR-012: Center (In Tune) Indicator
- FR-013: Sharp Indicator
- FR-014: Pure Major Third Reference Mark
- FR-015: Pure Minor Third Reference Mark
- FR-016: Cent Value Numeric Display
- FR-019: Display Update Rate
- FR-022: Meter Mode Activation
- FR-067: Mode Selection Control
- FR-068: Mode Selection Visibility
- FR-069: Settings Access Control
- FR-071: Visual Hierarchy in Meter Mode
- FR-073: Color Coding for Tuning Status
- FR-074: Touch Target Spacing
- FR-075: Safe Area Respect
- FR-078: Minimum Contrast Ratio

**Business Rules:**
- BR-004: Display update rate minimum 20 Hz
- BR-005: "IN TUNE" zone defined as ±5 cents

---

## Phase 4: Reference Tone Generation (Weeks 7-8)

### Objectives
- Implement sine wave synthesis
- Create tone generator with frequency control
- Add fade envelopes to prevent clicking
- Test output accuracy

### Implementation Steps

**Desktop Testing (Continuous):**
- Write unit tests for all new algorithmic code implemented in this phase
- Execute tests locally before committing
- Verify CI pipeline passes before proceeding to mobile device testing
- Target: Maintain ≥80% code coverage of shared library
- All new code in `src/shared/` must have corresponding tests in `tests/`

**Mobile Implementation:**

1. **Tone Generator Class**
   - Create `ToneGenerator` class for synthesis
   - Initialize with target frequency and sample rate
   - Implement phase accumulator for continuous waveform
   - Use double precision for phase to prevent drift

2. **Sine Wave Synthesis**
   - Implement: `sample = amplitude × sin(2π × f × t + φ)`
   - Calculate phase increment per sample
   - Accumulate phase across buffer boundaries
   - Normalize amplitude to -12 dBFS to -6 dBFS range

3. **Frequency Calculation from Note**
   - Map note selection (C4-C5) to frequency
   - Calculate: `f(n) = f_A4 × 2^((n - 9) / 12)`
   - Use current reference pitch for f_A4
   - Verify accuracy to 0.01 Hz

4. **Fade Envelope Implementation**
   - Add fade-in envelope (5-10ms) when tone starts
   - Add fade-out envelope (5-10ms) when tone stops
   - Use linear or cosine-shaped envelopes
   - Prevent clicks from abrupt starts/stops

5. **Tone State Management**
   - Implement toggle state: inactive/active
   - Handle activation: initialize generator, start output
   - Handle deactivation: apply fade-out, stop output
   - Support frequency change during playback (crossfade)

6. **Integration with Audio Callback**
   - Modify AudioProcessor to generate output when tone active
   - Write synthesized samples to output buffer
   - Ensure tone generation and pitch detection mutually exclusive
   - Verify real-time performance (no buffer underruns)

7. **Reference Pitch Integration**
   - Read current reference pitch from ConfigManager
   - Recalculate tone frequency when reference pitch changes
   - Apply smooth frequency transition if tone playing
   - Verify immediate application of changes

8. **Testing and Accuracy Validation**
   - Generate all 13 reference tones (C4-C5)
   - Measure output frequency with calibrated equipment
   - Verify accuracy within ±1.5 cents
   - Test amplitude stability (±0.5 dB)
   - Verify no harmonic content (spectral analysis)
   - Profile CPU usage (target ≤5%)

### Deliverables
- ToneGenerator class with synthesis algorithm
- Fade envelope implementation
- Frequency calculation verified
- Output accuracy validation report
- CPU usage measurements

### Success Criteria
- ✓ Output accuracy within ±1.5 cents for all 13 tones
- ✓ Pure sine wave (harmonics < -60 dB)
- ✓ Tone activation latency ≤10 ms
- ✓ No audible clicks on start/stop
- ✓ Amplitude stable within ±0.5 dB
- ✓ CPU usage ≤5% on minimum-spec devices
- ✓ Frequency updates smoothly when reference pitch changed

### Requirements Implemented
**Functional Requirements:**
- FR-024: Note Selection Range
- FR-028: Tone Generation Activation
- FR-029: Tone Generation Deactivation
- FR-030: Frequency Calculation
- FR-031: Tone Generation Accuracy
- FR-032: Waveform Synthesis
- FR-033: Amplitude Stability
- FR-035: Tone Continuity
- FR-036: Note Change During Playback
- FR-037: Reference Pitch Integration
- FR-038: Simultaneous Mode Exclusion
- FR-041: Tone Generation CPU Efficiency

**Business Rules:**
- BR-003: Reference tone accuracy within ±1.5 cents
- BR-008: Pitch detection and tone generation cannot operate simultaneously
- BR-009: Reference tone range limited to C4-C5
- BR-011: Generated tones must be pure sine waves
- BR-012: Tone activation latency ≤10ms
- BR-013: Amplitude must remain constant within ±0.5 dB
- BR-014: Fade envelopes required to prevent clicks
- BR-016: All tone frequencies calculated from current reference pitch

---

## Phase 5: Sound Mode UI and Integration (Week 9)

### Objectives
- Create note selection interface
- Implement tone toggle control
- Connect UI to tone generator
- Complete mode switching
- Maintain **measurement instrument on glass** design philosophy

### Design Philosophy Integration
Sound Mode must maintain the same minimal, professional aesthetic as Meter Mode. Controls should feel like instrument switches. The interface should present clear affordances for note selection without decoration or playful animations. All visual elements should communicate function, not personality.

### Implementation Steps

**Desktop Testing (Continuous):**
- Write unit tests for all new algorithmic code implemented in this phase
- Execute tests locally before committing
- Verify CI pipeline passes before proceeding to mobile device testing
- Target: Maintain ≥80% code coverage of shared library
- All new code in `src/shared/` must have corresponding tests in `tests/`

**Mobile Implementation:**

1. **Sound Mode Component Layout**
   - Create `SoundModeComponent` extending Component
   - Maintain dark background consistent with Meter Mode
   - Layout zones:
     - **Top:** Reference pitch display (same style as Meter Mode header)
     - **Middle:** Note selection interface (50% of screen area)
     - **Bottom:** Tone toggle control (prominent action)
     - **Mode selector:** Consistent position with Meter Mode
   - No scrolling; all elements visible simultaneously

2. **Note Selection Interface**
   - **Design options (choose one):**
     - Virtual keyboard: White/black key layout mirroring piano
     - Button grid: 13 buttons arranged logically (C4-C5)
   - **Appearance:**
     - Clean, geometric key shapes without decoration
     - No gradients, shadows, or 3D effects
     - Keys labeled clearly with note names
   - **Touch targets:**
     - Each selectable element ≥44×44 pt / 48×48 dp
     - Adequate spacing between keys to prevent mis-taps
   - **Visual feedback:**
     - Selected note highlighted with subtle color shift or border
     - No ripple effects or splash animations
     - Immediate visual response (opacity/color change on tap)
   - **Virtual keyboard specific:**
     - White keys: C, D, E, F, G, A, B, C (larger area)
     - Black keys: C#, D#, F#, G#, A# (smaller, offset above white keys)
     - Piano-like layout for intuitive mapping to musicians
     - No skeuomorphic 3D effects; flat design with clear outlines
   - **Button grid specific (alternative):**
     - 13 buttons in row or 2-row grid layout
     - Clear labeling for each note
     - Consistent sizing and spacing

3. **Reference Pitch Display (Top Zone)**
   - Display current A4 reference frequency (e.g., "440 Hz")
   - Same font and color treatment as Meter Mode reference frequency
   - Position top-center or top-left
   - Update dynamically when reference pitch changes in settings

4. **Tone Toggle Control (Bottom Zone)**
   - Create large button for tone activation/deactivation
   - **Button states:**
     - Inactive: neutral gray/off-white color, play icon (or simple label "Play")
     - Active: distinct but restrained color, stop icon (or label "Stop")
   - **Minimum size:** 60×60 dp (prominent action, largest control on screen)
   - **Visual treatment:** Flat, minimal icon style; outline icon preferred over filled
   - **Interaction:**
     - Single tap to toggle state
     - Tap-based only; no long-press or gesture behaviors
   - **Visual feedback:**
     - Subtle opacity change or color shift on press
     - No animation or bounce effects
     - Immediate state change indication
   - **Placement:** Centered at bottom of screen, with adequate margin from screen edge

5. **Toggle State Management**
   - Maintain boolean state: `toneActive`
   - Update button visual immediately on state change
   - Communicate state to AudioProcessor via thread-safe mechanism
   - Prevent UI blocking from audio thread updates

6. **Mode Switching Logic**
   - Implement mode enum: METER / SOUND
   - Handle mode selector tap events
   - Show/hide appropriate component based on mode (mutually exclusive)
   - **When switching from Sound to Meter:**
     - Stop tone automatically
     - Start pitch detection
     - Clear any pending audio state
   - **When switching from Meter to Sound:**
     - Stop pitch detection
     - Maintain previous note selection (if possible)
     - Preserve reference pitch value

7. **UI to Audio Connection**
   - Pass selected note to ToneGenerator
   - Pass toggle state to AudioProcessor
   - Handle reference pitch changes from settings
   - Update displayed reference pitch value
   - Ensure thread-safe state updates

8. **Visual Hierarchy and Design Consistency**
   - **Screen area allocation:**
     - Note selection: 50% of screen (primary interaction)
     - Tone toggle: 20% of screen (prominent action)
     - Reference pitch: 10% of screen (secondary information)
     - Spacing/margins: 20% (visual breathing room)
   - **Color consistency:**
     - Same dark background as Meter Mode
     - Off-white text for labels and information
     - Neutral grays for inactive state
     - Restrained accent color for active state (if used)
   - **Typography:**
     - Same sans-serif font family as Meter Mode
     - Consistent weight hierarchy
     - Note labels: medium weight
     - Reference pitch: light weight
   - **No decoration:** No gradients, shadows, borders, or rounded corners
   - **Controls feel like instrument switches:** Same visual weight and treatment across all interactive elements

### Deliverables
- Complete Sound Mode UI components
- Note selection interface (keyboard or grid)
- Tone toggle control
- Mode switching functionality
- Integrated audio and UI

### Success Criteria
- ✓ All 13 notes selectable with single tap
- ✓ Selected note visually highlighted
- ✓ Toggle button clearly indicates state
- ✓ Tone starts/stops within 10ms of button press
- ✓ Mode switching smooth and responsive
- ✓ Tone stops automatically when switching to Meter
- ✓ Reference pitch displayed and updates dynamically

### Requirements Implemented
**Functional Requirements:**
- FR-025: Note Selection Interface
- FR-026: Selected Note Indication
- FR-027: Tone Toggle Control
- FR-034: Output Volume Level
- FR-039: Speaker Output Routing
- FR-040: Background Deactivation
- FR-042: Audio Session Configuration
- FR-068: Mode Selection Visibility
- FR-072: Visual Hierarchy in Sound Mode

**Business Rules:**
- BR-015: Tone generation and pitch detection cannot operate simultaneously

---

## Phase 6: Settings and Calibration (Week 10)

### Objectives
- Create settings screen UI
- Implement reference pitch adjustment controls
- Add help documentation
- Test persistence and immediate application
- Maintain **measurement instrument on glass** design philosophy

### Design Philosophy Integration
Settings screen should maintain the minimal, professional aesthetic. Calibration controls should feel like precise instrument adjustments, not app preferences. Incremental adjustments with haptic feedback reinforce the instrument-like interaction model. Help documentation should educate without distraction.

### Implementation Steps

**Desktop Testing (Continuous):**
- Write unit tests for all new algorithmic code implemented in this phase
- Execute tests locally before committing
- Verify CI pipeline passes before proceeding to mobile device testing
- Target: Maintain ≥80% code coverage of shared library
- All new code in `src/shared/` must have corresponding tests in `tests/`

**Mobile Implementation:**

1. **Settings Screen Component and Navigation**
   - Create `SettingsComponent` extending Component
   - Maintain dark background consistent with Meter/Sound modes
   - Layout sections:
     - **Header:** Back button or close indicator
     - **Calibration section:** Reference pitch controls (primary)
     - **Help section:** Accessible help link or button
     - **About section:** App info and legal
   - Add back/close button (top-left or appropriate for platform)
   - Use platform-appropriate presentation (full-screen overlay or modal)
   - Handle settings button tap from main screen
   - Preserve previous mode state during settings visit

2. **Reference Pitch Display and Interaction**
   - Create centered, large text display: "440 Hz"
   - **Font size:** ≥6% of screen height, clean sans-serif, medium weight
   - **Color:** Off-white (neutral), same treatment as Meter Mode headers
   - **Update:** Display updates immediately on value change
   - **Presentation:** Centered horizontally, with adequate padding above and below

3. **Increment/Decrement Controls (Calibration)**
   - Create "−" button and "+" button for discrete steps
   - **Button positioning:** Flanking the frequency display horizontally
   - **Minimum size:** 44×44 pt / 48×48 dp each
   - **Increment:** 1 Hz per step
   - **Visual treatment:**
     - Flat, minimal icon or label style (outline preferred over filled)
     - Neutral gray when active
     - Grayed/disabled appearance when at limit
   - **Interaction:**
     - Single tap for one step
     - Tap-based only; consider long-press for continuous adjustment (optional)
   - **Haptic feedback:**
     - Soft, uniform haptic tap per discrete step (reinforces instrument-like calibration)
     - Haptic at each increment/decrement action
     - Respect system-level haptic disable settings
   - **Alternative: Stepped Slider**
     - If preferred, implement stepped slider with visible detents
     - Maintains discrete step behavior (not continuous)
     - Same 1 Hz increment per detent
     - Haptic feedback at each detent position

4. **Range Validation and Boundary Handling**
   - Enforce minimum: 410 Hz (disable "−" button at limit)
   - Enforce maximum: 480 Hz (disable "+" button at limit)
   - **Visual indication:**
     - Grayed appearance for disabled buttons
     - No action on tap when at boundary
     - Clear visual feedback that limit is reached
   - **Haptic feedback (optional):** Subtle feedback when at limit

5. **Reset Button**
   - Add "Reset to 440 Hz" button below increment/decrement controls
   - **Visual treatment:** Flat, minimal style, same weight as +/− buttons
   - **Positioning:** Centered or flanking with +/− buttons
   - **Minimum size:** 44×44 pt / 48×48 dp
   - **Interaction:** Single tap to restore default value
   - **Feedback:** Display updates immediately; optional haptic tap

6. **Value Persistence and Application**
   - Write to ConfigManager on each change (not just on exit)
   - Handle storage write failures gracefully (retry or notify user)
   - Verify persistence across app restarts
   - **Immediate application:**
     - Update FrequencyCalculator with new reference immediately
     - Recalculate all target frequencies
     - If in Meter Mode: update cent deviations in real-time
     - If in Sound Mode with tone playing: transition frequency smoothly (crossfade)

7. **Help Documentation Screen**
   - Create `HelpComponent` with clear, concise content
   - **Dark background:** Consistent with main UI
   - **Scrollable content:** For devices with limited height
   - **Sections:**
     - **Note Names:** Simple chromatic scale reference (table or list)
     - **Cents Explained:** What cents are, how they represent pitch deviation
     - **Tuning Accuracy:** Explanation of ±1 cent target and why it matters
     - **Pure Intervals:** Meaning of pure major/minor third reference marks
     - **Reference Pitch:** Purpose of calibration, when to adjust (concert pitch contexts)
     - **Usage Tips:** Quiet environment, microphone positioning, avoiding wind noise
   - **Typography:** Light weight, off-white text, ample line spacing
   - **No decoration:** Simple bullet points or short paragraphs
   - **Navigation:** Back button or close to return to settings

8. **About Section**
   - Display app name ("SimpleTuner") and version number
   - Add copyright information ("© [Year] [Author/Organization]")
   - Brief statement of app purpose (1-2 sentences)
   - Link to privacy policy (text, not button) if applicable
   - **Optional:** Credits and acknowledgments (JUCE framework mention)
   - **Typography:** Light weight, off-white text, minimal visual emphasis

9. **Settings Visual Design Summary**
   - **Color consistency:** Dark background, off-white text, neutral grays
   - **Typography:** Same sans-serif family as main UI, light weight for secondary text
   - **Spacing:** Generous margins between sections for clarity
   - **No decoration:** No gradients, shadows, icons beyond minimal labels
   - **Touch targets:** All interactive elements meet minimum size guidelines
   - **Feedback:** Subtle visual change on tap; haptic feedback for calibration steps

### Deliverables
- Complete settings screen UI
- Reference pitch adjustment controls
- Help documentation content
- Settings persistence verification
- Immediate application of changes

### Success Criteria
- ✓ Reference pitch adjustable from 410-480 Hz in 1 Hz increments
- ✓ Value persists across app restarts
- ✓ Changes apply immediately without restart
- ✓ Boundary enforcement working correctly
- ✓ Help content accessible and comprehensible
- ✓ Settings accessible within 2 taps from main screen

### Requirements Implemented
**Functional Requirements:**
- FR-043: Reference Pitch Range
- FR-044: Reference Pitch Increment
- FR-045: Default Reference Pitch
- FR-046: Reference Pitch Display
- FR-047: Increment Control
- FR-048: Decrement Control
- FR-050: Reset to Default
- FR-052: Immediate Application
- FR-053: Frequency Recalculation
- FR-054: Persistent Storage
- FR-055: Storage Reliability
- FR-056: Settings Access
- FR-057: Visual Feedback During Adjustment
- FR-059: A4 Frequency Display Precision
- FR-070: Help Access Control

**Business Rules:**
- BR-001: Reference pitch A4 between 410-480 Hz
- BR-017: Reference pitch adjustment increments 1 Hz
- BR-018: Default reference pitch 440 Hz
- BR-019: Reference pitch changes apply immediately
- BR-020: Reference pitch persists across launches

---

## Phase 7: Orientation and Layout Polish (Week 11)

### Objectives
- Implement responsive layout for all orientations
- Test on varied screen sizes
- Polish visual design
- Optimize layout transitions

### Implementation Steps

**Desktop Testing (Continuous):**
- Write unit tests for all new algorithmic code implemented in this phase
- Execute tests locally before committing
- Verify CI pipeline passes before proceeding to mobile device testing
- Target: Maintain ≥80% code coverage of shared library
- All new code in `src/shared/` must have corresponding tests in `tests/`

**Mobile Implementation:**

1. **Orientation Detection**
   - Register for orientation change notifications
   - Detect portrait vs landscape orientation
   - Handle orientation-specific layouts

2. **Portrait Layout Implementation**
   - Meter Mode: vertical stack (mode selector, note, meter, status)
   - Sound Mode: vertical stack (mode selector, ref pitch, keyboard, toggle)
   - Settings: vertical scrollable layout
   - Test on various screen heights

3. **Landscape Layout Implementation**
   - Meter Mode: horizontal distribution (note left, meter center-right)
   - Sound Mode: keyboard spans width, toggle bottom-center
   - Settings: possibly two-column layout if space permits
   - Test on various screen widths

4. **Layout Transition Logic**
   - Implement resized() callback for dynamic layout
   - Calculate element positions based on available bounds
   - Use responsive sizing (percentages, not fixed pixels)
   - Animate transitions smoothly (optional)

5. **Screen Size Adaptation**
   - Test on small screens (4.7" iPhone SE)
   - Test on standard screens (6.1" typical)
   - Test on large screens (6.7" iPhone Pro Max)
   - Verify no clipping, overlap, or illegible text

6. **Aspect Ratio Testing**
   - Test 16:9 (older devices)
   - Test 18:9, 19:9, 19.5:9 (modern Android)
   - Test 20:9 (some recent devices)
   - Ensure layout adapts gracefully

7. **Safe Area Handling**
   - Query safe area insets from platform
   - Adjust layout to avoid notches, rounded corners, home indicators
   - Test on devices with notches (iPhone X+) and hole-punch cameras
   - Ensure controls not obscured by system UI

8. **Visual Design Polish**
   - Refine color palette consistency
   - Adjust font sizes for optimal readability
   - Fine-tune meter needle animation damping
   - Add subtle shadows or borders for depth (optional)
   - Ensure platform-appropriate styling (iOS vs Android)

9. **Performance Optimization**
   - Profile layout calculation performance
   - Optimize paint() methods to avoid unnecessary redraws
   - Ensure orientation changes complete within 200ms
   - Verify no audio interruption during orientation change

10. **Accessibility Improvements**
    - Add accessible labels to UI elements (for screen readers)
    - Ensure touch targets have sufficient spacing
    - Test with VoiceOver (iOS) and TalkBack (Android)
    - Document accessibility limitations (meter difficult for blind users)

### Deliverables
- Responsive layouts for portrait and landscape
- Screen size adaptation verified
- Visual design polished
- Accessibility improvements
- Performance optimization

### Success Criteria
- ✓ Orientation changes complete within 200ms
- ✓ No audio interruption during orientation change
- ✓ Layout correct on all tested screen sizes (4.7"-6.8")
- ✓ Safe areas respected on notched devices
- ✓ All touch targets meet size guidelines with spacing
- ✓ Basic screen reader compatibility functional
- ✓ Visual design consistent and platform-appropriate

### Requirements Implemented
**Functional Requirements:**
- FR-060: Portrait Orientation Support
- FR-061: Landscape Orientation Support
- FR-062: Dynamic Orientation Switching
- FR-063: Orientation Transition Performance
- FR-064: Layout Preservation During Orientation Change
- FR-065: Responsive Element Sizing
- FR-066: Screen Size Adaptation
- FR-076: Status Bar Handling
- FR-077: System UI Integration
- FR-079: Font Scaling Support
- FR-082: Visual Feedback for Interactions

---

## Phase 8: Platform-Specific Integration and Testing (Week 12)

### Objectives
- Finalize platform-specific implementations
- Conduct comprehensive testing
- Fix critical bugs
- Prepare for app store submission

### Implementation Steps

**Desktop Testing (Continuous):**
- Write unit tests for all new algorithmic code implemented in this phase
- Execute tests locally before committing
- Verify CI pipeline passes before proceeding to mobile device testing
- Target: Maintain ≥80% code coverage of shared library
- All new code in `src/shared/` must have corresponding tests in `tests/`

**Mobile Implementation:**

1. **iOS-Specific Implementation**
   - Configure AVAudioSession categories correctly
   - Handle iOS audio interruptions (calls, alarms)
   - Test background/foreground transitions
   - Verify UserDefaults persistence
   - Test on iOS 13, 15, 16, 17

2. **Android-Specific Implementation**
   - Configure AudioManager usage correctly
   - Handle Android audio focus
   - Test background/foreground transitions
   - Verify SharedPreferences persistence
   - Test on Android 8, 10, 12, 14

3. **Permission Flow Testing**
   - Test first launch permission request
   - Test permission granted flow
   - Test permission denied handling
   - Test "Open Settings" link functionality
   - Verify Sound Mode works without mic permission

4. **Audio Route Change Handling**
   - Test headphone connect/disconnect during use
   - Test Bluetooth headset pairing/unpairing
   - Verify smooth audio route transitions
   - Ensure tone/detection continues on new route

5. **Interruption Handling**
   - Simulate phone calls during use
   - Test alarm/notification interruptions
   - Verify correct pause and resume behavior
   - Test with other apps starting audio

6. **Memory and Performance Testing**
   - Run extended sessions (30+ minutes)
   - Monitor memory usage with profilers
   - Check for memory leaks
   - Verify CPU usage stays within limits
   - Measure battery impact (temperature)

7. **Accuracy Validation**
   - Generate calibrated test tones across frequency range
   - Measure pitch detection accuracy (target ±1 cent)
   - Measure tone generation accuracy (target ±1.5 cents)
   - Test at boundary frequencies (C1, C8)
   - Document results in validation report

8. **Cross-Device Testing**
   - Test on minimum 3 iOS devices (iPhone SE, 12, 14)
   - Test on minimum 5 Android devices (varied manufacturers)
   - Verify functionality on low-end devices
   - Test performance on high-end devices
   - Document device compatibility matrix

9. **Bug Fixing and Stabilization**
   - Prioritize critical and high-severity bugs
   - Fix crashes discovered during testing
   - Address performance issues on low-end devices
   - Resolve usability issues from testing
   - Verify fixes don't introduce regressions

10. **App Store Preparation**
    - Verify Info.plist configuration (iOS)
    - Verify AndroidManifest.xml configuration
    - Prepare app icons (all required sizes)
    - Create screenshots for app store listings
    - Write app store descriptions
    - Prepare privacy policy (minimal data collection)
    - Test installation from archive/APK

### Deliverables
- Platform-specific implementations complete
- Comprehensive test results
- Bug fixes for critical issues
- Accuracy validation report
- Device compatibility matrix
- App store submission materials

### Success Criteria
- ✓ All critical requirements tested and passing
- ✓ Pitch detection accuracy ≥95% within ±1 cent (40+ dB SNR)
- ✓ Tone generation accuracy 100% within ±1.5 cents
- ✓ Crash rate <0.5% in testing sessions
- ✓ Performance requirements met on all tested devices
- ✓ Audio interruption handling working correctly
- ✓ Permission flows tested on both platforms
- ✓ No critical or high-severity open bugs
- ✓ App store guidelines compliance verified

### Requirements Implemented
**Functional Requirements:**
- FR-085: Permission Denial Handling
- FR-090: Input Level Monitoring
- FR-091: Automatic Gain Control Handling
- FR-092: Audio Route Detection
- FR-093: Audio Route Change Handling
- FR-094: Audio Session Category Configuration
- FR-095: Audio Interruption Handling
- FR-096: Background Audio Suspension
- FR-097: Foreground Audio Resumption
- FR-098: Audio Device Error Handling
- FR-099: CPU Load Monitoring
- FR-102: Thread Priority Management
- FR-103: Audio Focus Management (Android)
- FR-104: Silent Mode Handling (iOS)

**Business Rules:**
- BR-010: Microphone permission required for Meter Mode
- BR-015: Audio processing suspends when backgrounded

---

## Testing Strategy

### Desktop Unit Testing (Phase 0: Continuous, Weeks 1-12)

**Primary Testing Approach:**
- Desktop tests execute **first** on every code change
- Fast feedback loop (test suite executes in <60 seconds)
- Run locally by developers before commit
- Run automatically in CI/CD on every push
- **Gate mobile builds:** Mobile compilation/testing only proceeds if desktop tests pass

**Coverage Targets:**
- ≥80% code coverage of shared library (`src/shared/`)
- ≥95% of core algorithm code in shared library
- 100% of critical algorithms covered by unit tests

**Test Scope:**
- Pitch detection algorithm (autocorrelation, NSDF, frequency extraction, note mapping)
- Tone generation algorithm (sine synthesis, frequency calculation, phase accumulation)
- Frequency calculations (equal temperament, cent deviation, octave relationships)
- Configuration management (reference pitch storage, retrieval, validation)
- Signal validation (amplitude thresholds, confidence scoring)

**Test Infrastructure:**
- **Build System:** CMake
- **Test Framework:** Google Test
- **Mock Platform Services:** MockAudioInput, MockAudioOutput, MockConfigStorage, MockPermissions
- **Platforms:** Linux (developer workstations + CI runners)

**Execution:**
1. Developer writes feature code in `src/shared/`
2. Developer writes unit tests in `tests/`
3. Developer runs `make test` locally
4. Developer commits code
5. CI/CD runs desktop test suite
6. If tests pass → CI proceeds to mobile builds
7. If tests fail → CI stops, notifies developer, blocks merge

**Metrics:**
- Test execution time: <60 seconds (target: <30 seconds)
- Code coverage: ≥80% of shared library
- Test count: ≥50 unit tests by project completion

---

### Mobile Integration Testing (Phase 1: Weeks 3-4)

**Entry Criteria:** Desktop unit tests passing at ≥80% coverage

**Scope:**
- Audio pipeline end-to-end on real devices
- UI to audio communication
- Platform-specific integrations (permissions, storage, audio I/O)
- Mode switching with audio state changes
- Settings changes propagating correctly

**Execution:**
- QA team on physical devices (iOS: iPhone SE, 12, 14; Android: varied manufacturers)
- Manual testing with real audio input/output
- Automated where feasible (XCTest, Android Instrumentation)

**Pass Criteria:**
- Audio I/O functional on all test devices
- Platform integrations working correctly
- No regressions from desktop test results

---

### System Testing (Phase 2: Weeks 5-7)

**Entry Criteria:** Desktop unit tests passing + integration tests passing

**Scope:**
- Comprehensive functional test suite on mobile devices
- Accuracy validation with calibrated equipment
- Performance profiling (latency, CPU, memory)
- Cross-device compatibility (8+ devices spanning iOS/Android, budget/flagship)
- Usability testing with target users

**Execution:**
- Parallel testing on multiple devices
- Calibrated audio equipment for accuracy measurement
- Performance profiling tools
- Real-world environmental conditions

**Pass Criteria:**
- All CRITICAL and HIGH priority tests passing
- Performance requirements met (latency, CPU, memory)
- Accuracy requirements met (±1 cent detection, ±1.5 cent generation)

---

### Beta Testing (Phase 3: Weeks 8-10)

**Entry Criteria:** Desktop + integration + system tests passing

**Scope:**
- 50-100 volunteer musicians using app in real-world scenarios
- Crash reporting via platform tools (Crashlytics, Sentry)
- User feedback collection
- Real-world device/environment diversity

**Pass Criteria:**
- Crash rate <0.5% of sessions
- User satisfaction >80%
- No critical issues blocking release

---

### Acceptance Testing (Phase 4: Week 11)

**Entry Criteria:** All previous phases passing

**Scope:**
- User acceptance testing with representative users (amateur, professional, educators)
- Final accuracy validation
- App store compliance verification
- All critical requirements implemented

**Pass Criteria:**
- UAT criteria met (80%+ success rates)
- App store guidelines compliance verified
- Release checklist complete

---

### CI/CD Pipeline Summary

**Stage 0: Desktop Unit Tests (Every Commit)**
- Build shared library via CMake
- Build test executable
- Execute unit tests
- Report coverage metrics
- **Duration:** 2-5 minutes
- **Gate:** Blocks all subsequent stages if failed

**Stage 1: iOS Build Verification (After Stage 0 Passes)**
- Generate Xcode project via Projucer
- Compile iOS build
- **Duration:** 10-15 minutes
- **Gate:** Blocks iOS deployment if failed

**Stage 2: Android Build Verification (After Stage 0 Passes)**
- Generate Android Studio project via Projucer
- Compile Android build
- **Duration:** 10-15 minutes
- **Gate:** Blocks Android deployment if failed

**Stage 3: Mobile Device Tests (Manual/Scheduled)**
- Integration tests on physical devices
- System tests with calibrated equipment
- **Duration:** Hours (manual testing)
- **Gate:** Blocks release if failed

---

## Risk Management

| Risk | Mitigation Strategy |
|------|-------------------|
| Android device audio variability | Test on diverse devices early; design robust algorithms |
| Autocorrelation performance on low-end devices | Optimize algorithm; consider adaptive window size |
| Platform OS updates breaking functionality | Monitor beta releases; test preemptively |
| Accuracy not meeting ±1 cent target | Allocate extra time for algorithm tuning in Phase 2 |
| Microphone quality variations | Document limitations; set appropriate user expectations |
| App store rejection | Review guidelines thoroughly; pre-submission checklist |

---

## Success Metrics

**Desktop Testing Metrics:**
- Code coverage: ≥80% of shared library
- Test execution time: <60 seconds (target: <30 seconds)
- Test count: ≥50 unit tests covering core algorithms
- CI/CD success rate: ≥95% of commits pass desktop tests on first run
- Shared library isolation: ≥95% of core algorithm code in `src/shared/`

**Technical Metrics:**
- Pitch detection accuracy: ≥95% within ±1 cent (40+ dB SNR)
- Tone generation accuracy: 100% within ±1.5 cents
- Latency: ≤30ms pitch detection, ≤10ms tone generation
- CPU usage: ≤15% (Meter), ≤5% (Sound)
- Memory: ≤50 MB peak
- Crash rate: <0.5% of sessions

**User Metrics (from beta testing):**
- 80%+ of amateur musicians successfully tune within 5 minutes
- 90%+ of professional musicians report accuracy meets standards
- 80%+ of users report interface is clear and easy to use

**Business Metrics:**
- App store approval on both platforms
- Installation size ≤25 MB
- 100% offline functionality verified
- All critical requirements implemented

---

## Deliverables Summary

**Phase 1 (Week 1):** Desktop CMake build, platform abstraction interfaces, mock implementations, unit test framework, CI/CD pipeline (Stage 0)
**Phase 1 (Week 2):** Mobile Projucer builds, JUCE platform implementations, basic audio I/O, permission flow, CI/CD Stages 1-2
**Phase 2:** Pitch detection algorithm, frequency-to-note mapping, accuracy validation
**Phase 3:** Complete Meter Mode UI with tuning meter
**Phase 4:** Tone generation algorithm, output accuracy validation
**Phase 5:** Complete Sound Mode UI with note selection
**Phase 6:** Settings screen, calibration controls, help documentation
**Phase 7:** Responsive layouts, orientation handling, visual polish
**Phase 8:** Platform integration, comprehensive testing, app store submission materials

---

## Timeline Summary

| Week | Phase | Major Activities |
|------|-------|-----------------|
| 1 | Phase 1a | **Desktop testing infrastructure: CMake, abstractions, mocks, CI/CD** |
| 2 | Phase 1b | **Mobile project setup, JUCE integration, platform implementations** |
| 3-4 | Phase 2 | Pitch detection algorithm implementation and testing (desktop + mobile) |
| 5-6 | Phase 3 | Meter Mode UI implementation (mobile) |
| 7-8 | Phase 4 | Tone generation implementation and testing (desktop + mobile) |
| 9 | Phase 5 | Sound Mode UI and mode switching (mobile) |
| 10 | Phase 6 | Settings, calibration, help documentation (mobile) |
| 11 | Phase 7 | Orientation handling, layout polish (mobile) |
| 12 | Phase 8 | Platform integration, testing, app store prep |

**Note:** Desktop unit testing runs continuously throughout all weeks as features are implemented.

**Total Duration:** 12 weeks from project start to app store submission

This implementation plan provides a structured, incremental approach to building the Chromatic Tuner application with clear milestones, testable success criteria, and full traceability to requirements.
