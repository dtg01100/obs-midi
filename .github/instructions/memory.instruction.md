---
applyTo: '**'
---

# User Memory

## User Preferences
- Programming languages: C++ (Qt), CMake
- Code style preferences: match existing project conventions
- Development environment: VS Code in devcontainer (Ubuntu 22.04)
- Communication style: concise, engineer-focused

## Project Context
- Repository: obs-midi (OBS Studio plugin)
- Tech stack: C++17/C++20, Qt5, libremidi (submodule), libobs/obs-frontend-api
- Key files:
  - src/obs-midi.cpp (entry)
  - src/midi-agent.* (libremidi glue)
  - src/libremidi (submodule)

## Conversation History
- Performed submodule init for libremidi and pinned to a version compatible with the plugin
- Installed Ubuntu build dependencies via CI/install-dependencies-ubuntu.sh
- Patched libremidi CMake helper to avoid unknown CMake helper
- Implemented a compatibility shim to adapt libremidi v4 API to the legacy API used by this plugin
- Added compatibility shims for missing obs-frontend-api functions (no-op fallbacks)

## Context7 Research History
- None (no external Context7 calls were needed during this session)

## Notes
- Built plugin successfully in container: build artifact at /workspace/build/obs-midi.so
- Compatibility shims are conservative; runtime behavior may differ from native OBS functions that were stubbed.
