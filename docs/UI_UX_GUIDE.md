# SimpleTuner UI/UX Guide

## Purpose
This document defines the **UI and UX principles** for a mobile tuner app derived from a dedicated hardware tuner, reinterpreted to feel **native to touchscreen mobile devices**. The design emphasizes **precision, clarity, and restraint**, positioning the app as a professional instrument rather than a consumer or lifestyle product.

The entire phone screen is treated as the interface. There is no representation of a physical enclosure.

---

## Core Design Philosophy

- The app behaves like a **measurement instrument on glass**
- Visual hierarchy favors **accuracy over decoration**
- Interaction is deliberate and minimal
- Feedback (visual, motion, haptic) is informational, never expressive

The user should trust the app immediately, without needing to explore or learn it.

---

## Visual Foundation

### Background
- Full-screen **dark background** (charcoal or near-black, not pure black)
- Matte appearance
- No visible texture, noise, or strong gradients

Purpose: reduce eye fatigue and allow tuning indicators to stand out clearly.

---

## Layout Structure

### Orientation
- Primary: **landscape**
- Secondary: portrait (reflowed layout, not scaled)

### Landscape Layout Zones

1. **Top Zone: Note & Status**
2. **Center Zone: Tuning Meter (Primary Focus)**
3. **Bottom Zone: Controls & Calibration**

No scrolling. All core information is visible at all times.

---

## Primary Interface Element: Tuning Meter

### Role
The tuning meter is the **visual and functional anchor** of the app. All other elements support it.

### Design
- Wide arc or semi-circular meter spanning most of the screen width
- Thin, precise tick marks
- Center point labeled clearly as **0**
- Scale represents cents deviation (e.g. ±20)

### Indicator
- Thin vertical line or narrow wedge
- Smooth, damped motion
- No bouncing or spring effects

### Color Behavior
- Neutral gray or off-white when far from center
- Subtle transition to restrained green near perfect tuning
- No flashing or pulsing

---

## Note Display (Top Zone)

### Note Name
- Large, bold, centered
- Single letter dominant (e.g. A)
- Accidentals (♯ / ♭) smaller and aligned cleanly

### State Color
- Neutral white when unstable
- Soft green when stably in tune

### Supporting Information
- Reference pitch (e.g. 440 Hz)
- Smaller text
- Low visual emphasis

---

## Controls and Interaction (Bottom Zone)

### Calibration Control
- Displayed numerically (e.g. "440 Hz")
- Tap to adjust
- Adjustment presented as:
  - Discrete +/- steps, or
  - A stepped slider with visible detents

The interaction should feel **incremental and precise**, not continuous or playful.

### Sound / Reference Tone
- Simple toggle
- Flat, minimal iconography
- Same visual weight as other controls

### Advanced Modes
- Accessed via a subtle label (e.g. "Mode" or "Advanced")
- Interval or specialty tuning info appears only when enabled
- Secondary to the main tuning task

---

## Touch Interaction Model

- Tap-based interactions only
- No reliance on gestures
- No hidden controls
- Clear visual feedback on press (subtle highlight or opacity change)

Controls should feel like **instrument switches**, not app buttons.

---

## Typography

- Clean, modern sans-serif
- Neutral, technical character
- Weight hierarchy:
  - Heavy: Note name
  - Medium: Labels
  - Light: Calibration and secondary info

Avoid rounded, playful, or decorative fonts.

---

## Color System

- Background: dark charcoal / near-black
- Primary text: off-white
- Active/in-tune indication: restrained green
- Inactive elements: muted gray

Color communicates **state**, not emotion.

---

## Motion and Animation

- Only used where it conveys measurement
- Meter movement is continuous and physically believable
- UI state changes are near-instant
- No decorative or attention-seeking animation

---

## Haptic Feedback Strategy (Limited Scope)

Haptics are used sparingly to reinforce **intentional interaction**, not tuning accuracy.

### Calibration Adjustment

**Trigger**
- Each discrete step when changing reference pitch (e.g. 440 → 441 Hz)

**Haptic Pattern**
- Soft, uniform tap per step

**Purpose**
- Reinforces discrete, instrument-like calibration steps

---

### Mode or Sound Toggle

**Trigger**
- Toggling reference tone on/off
- Switching tuning modes or interval settings

**Haptic Pattern**
- Single neutral tap
- Identical feedback for all toggles

**Purpose**
- Confirms intentional interaction without adding hierarchy or emphasis

---

### Haptic Constraints

- No haptics tied to pitch accuracy
- No repeated or continuous vibration
- Respect system-level haptic disable settings

---

## Portrait Mode Adaptation

- Note name at top
- Meter reflows into a wide horizontal arc or linear meter
- Controls stacked below
- Maintain visual dominance of tuning accuracy

Portrait mode is a considered layout, not a scaled version.

---

## Intended User Experience

The app should feel:
- Calm
- Reliable
- Focused
- Professional

The user’s attention stays on sound and tuning, not on the interface itself.

---

## Design Summary

A dark, glass-native tuning instrument that uses a restrained analog meter metaphor, precise typography, minimal touch controls, and subtle haptics to deliver accuracy without distraction.

