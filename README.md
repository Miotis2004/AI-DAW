# AI-DAW

AI-DAW is an ambitious Digital Audio Workstation project built using the JUCE C++ framework. It aims to integrate modern AI-driven audio processing with a flexible modular architecture.

## Project Vision

The application is designed to fulfill three primary functions:

1.  **Modular Guitar Effects Board**: A flexible, pedalboard-style GUI allowing users to chain various effects.
2.  **Amp Modeler**: Implementation of high-quality amplifier modeling, targeting integration with technologies like **NAM (Neural Amp Modeler)**.
3.  **Digital Audio Workstation (DAW)**: A full-featured recording and mixing environment.

## Technical Overview

-   **Framework**: JUCE (C++)
-   **Architecture**:
    -   Currently initialized as a GUIP application.
    -   Migrating towards a `juce::AudioProcessorGraph` backend to support the modular signal chain requirements.

## Current State

The project is currently in the **initialization phase**.
-   Basic JUCE project structure (`.jucer`, Source files).
-   Basic Audio Application scaffolding (Audio I/O enabled).
