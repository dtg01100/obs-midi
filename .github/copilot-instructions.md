## Quick orientation — what this repository is

This is an OBS Studio plugin (obs-midi) that maps MIDI device input/output to OBS actions and events.
It is written in C++17, uses Qt 5 (Core + Widgets) for the UI, libremidi for MIDI I/O, and the OBS C API / obs-frontend-api.

## Big-picture architecture (files that show the flow)
- Entry point: `src/obs-midi.cpp` — registers plugin, creates `PluginWindow`, `Config`, `DeviceManager` and `Events`.
- MIDI device lifecycle: `src/device-manager.cpp` (manages MidiAgent instances) -> `src/midi-agent.cpp` (libremidi callbacks).
- MIDI mapping / hooks: `src/Midi_hook.cpp` (MidiMapping stores hook JSON and translates to Actions).
- Action dispatch: `src/obs-controller.cpp` (Actions map — add new OBS actions here; these classes call OBS frontend API).
- OBS events / RPC: `src/events.cpp` and `src/rpc/RpcEvent.*` (observes OBS frontend events and emits RPC-style updates).
- Configuration: `src/config.cpp` (persists plugin state using OBS' obs_data store). Config files are named per-profile/scene-collection.

Typical data flow: MIDI device -> libremidi callback (MidiAgent) -> MidiMapping match -> Actions (obs-controller) -> OBS API / Events -> serialized to config or emitted via RPC.

## Developer workflows — build / run / debug
- Primary dev workflow: build the plugin inside an OBS Studio source tree. See `build.md` for step-by-step instructions.
- Common quick Linux example (from `build.md`):
  - mkdir build && cd build
  - cmake -DLIBOBS_INCLUDE_DIR="<path to obs-studio>/libobs" -DCMAKE_INSTALL_PREFIX=/usr -DUSE_UBUNTU_FIX=true ..
  - make -j4 && sudo make install
- If you already have an obs-studio build, prefer adding `add_subdirectory(obs-midi)` to `{obs-studio}/plugins/CMakeLists.txt` and re-configuring with CMake GUI / CLI (CMakeLists shows required variables like `LIBOBS_INCLUDE_DIR`, `OBS_FRONTEND_LIB`).
- macOS/iOS/CI: scripts in `CI/` are used by the project's automated builds (see `CI/*.sh` and `CI/install-*` scripts). Use them when reproducing CI builds.

## Where state/config lives and important conventions
- Plugin config is written via OBS' obs_data APIs. Files are saved in the obs-module config path with name pattern: `obs-midi_{profile}_{scene_collection}.json` (see `src/config.cpp::get_file_name`).
- When changing config programmatically call `GetConfig().get()->Save()` so the obs JSON is persisted.
- The device list and hooks are serialized as OBS JSON (see `DeviceManager::GetData()` and `MidiAgent::GetData()`). Use `obs_data_create_from_json` / `obs_data_get_*` helpers everywhere.

## How to add or change mappings and actions (concrete)
- Mappings are stored as JSON objects and deserialized by `src/Midi_hook.cpp` into `MidiMapping` instances.
- To add a new OBS-triggered action: edit `src/obs-controller.cpp` — add a mapping in `Actions::make_map()` (string key -> new Action subclass). Implement the Action subclass methods (parse data in `parse_data()` and perform the effect in `execute()`).
  - Example: many actions call `obs_frontend_*` APIs (see `SetCurrentScene::execute()` and `ToggleMute::execute()`). Follow the same pattern and use `obs_queue_task(OBS_TASK_UI, ...)` for UI-thread operations if needed.

## Concurrency and thread-safety notes agents must know
- libremidi callbacks run off the main UI thread. MidiAgent sets libremidi callbacks that call static methods (`MidiAgent::HandleInput`) which emit Qt signals (e.g. `broadcast_midi_message`). Many handlers immediately call OBS API — keep OBS API calls on the main thread where required.
- The project already uses Qt signals/slots and occasionally `obs_queue_task(OBS_TASK_UI, ...)` for UI-thread work. Copy those patterns when adding threaded behavior.

## Integration points and external dependencies
- libremidi (bundled under `src/libremidi`) — MIDI I/O and exceptions (see `midi-agent.cpp`).
- OBS native APIs: `libobs`, `obs-frontend-api` — C ABI usage throughout (see `CMakeLists.txt` target_link_libraries and files in `src/*` referencing `obs_*` functions).
- Qt 5.15 (Core + Widgets) — UI in `src/forms/` and QObject-based classes.

## Common project-specific idioms to mirror
- Data interchange uses OBS' obs_data JSON (not arbitrary protobufs). Use obs_data getters/setters for serialization.
- UI and lifecycle objects are QObjects parented to `PluginWindow` or `DeviceManager` — rely on Qt parent-child deletion semantics.
- Actions are constructed from `MidiMapping` using string identifiers — keep mapping strings stable when refactoring (they are persisted in user config JSON).

## Files to inspect first when debugging or extending
- `src/obs-midi.cpp` — plugin lifecycle and wiring
- `src/midi-agent.cpp`, `src/device-manager.cpp` — MIDI device lifecycle and callbacks
- `src/Midi_hook.cpp` — JSON -> MidiMapping parsing/serialization
- `src/obs-controller.cpp` — where OBS actions live; add new action classes here
- `src/config.cpp` — persisting and loading state (per-profile + per-scene-collection filenames)
- `CI/` and `build.md` — reproducible build steps and platform scripts

## Quick pitfalls / gotchas
- Config filenames include the current profile and scene collection; tests/quick reloads should account for this (see `Config::get_file_name`).
- Many OBS functions expect calls on OBS/UI thread. If you see crashes/warnings, verify use of `obs_queue_task` or that the call happens on the main thread.
- Mapping string keys in `Actions::make_map()` are user-visible and saved; renaming them will orphan saved mappings unless you add a migration.

If any of the above sections are unclear or you'd like added examples (e.g. step-by-step to add a specific new action and test it), tell me which part and I will expand that section and iterate. 
