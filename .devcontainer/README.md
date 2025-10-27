Devcontainer for obs-midi

What this provides
- Ubuntu 22.04 image with C/C++ build toolchain, CMake, Ninja, Qt5 development packages and common debugging tools (gdb, lldb).

Workspace availability
- The local repository is mounted into the container at `/workspace` by the devcontainer configuration (`workspaceMount`). If you open the container in VS Code the project files will be available at `/workspace` inside the container.

Notes and caveats
- Building obs-midi requires an OBS Studio development tree (libobs and obs-frontend-api). The container does not automatically fetch or build OBS Studio (that is a large, platform-specific step).
- Typical approach: open this repository in the container and also mount or clone an OBS Studio source tree alongside this workspace (for example /workspace/obs-studio). Then follow the steps in `build.md` inside the obs-studio tree (add_subdirectory(obs-midi)).

Quick start
1. Open the repository in VS Code and choose "Reopen in Container".
2. (Optional) Clone or mount your OBS Studio source tree next to this workspace. Example layout inside the container:

   /workspace/obs-midi  <-- this repo (workspace)
   /workspace/obs-studio <-- your local obs-studio checkout (required for building)

3. Build inside your obs-studio tree. From a terminal in the container:

   mkdir -p /workspace/obs-studio/build && cd /workspace/obs-studio/build
   cmake -G "Unix Makefiles" -S .. -B . -DCMAKE_BUILD_TYPE=Debug -DOBS_PLUGIN_DIR=/usr ..
   # make or ninja depending on your generator

Or follow the repo's `build.md` for recommended platform-specific steps.

When editing
- Add new OBS actions in `src/obs-controller.cpp`.
- MIDI mappings are stored as OBS JSON (see `src/Midi_hook.cpp`).
- Config files are written to the OBS module config path with pattern `obs-midi_{profile}_{scene_collection}.json` (see `src/config.cpp`).

Qt6 porting notes
- The devcontainer now includes both Qt5 and Qt6 development packages so you can compile and test code against either.
- Typical changes when porting to Qt6:
   - Update CMake find_package(Qt6 COMPONENTS Widgets REQUIRED) and replace Qt5::Widgets with Qt6::Widgets in targets.
   - Inspect UI/forms that include Qt5 headers (e.g., `#include <QtWidgets/QAction>`) — most Qt5 headers map to the same Qt6 headers but some modules have moved or renamed symbols.
   - Check `src/forms/` for generated UI code (uic) and regenerate with Qt6 uic if you change .ui files.
   - Update any code using Qt5-specific helper functions to their Qt6 equivalents — consult Qt6 docs for exact mappings.
- Files to inspect when porting: `CMakeLists.txt`, `src/obs-midi.cpp`, `src/forms/*`, `src/obs-controller.cpp`.

Building for Qt6
- You may need to update the CMake configuration used when building inside an obs-studio tree to prefer Qt6 (change find_package calls in your build invocation). The devcontainer provides Qt6 headers so tests and edit/compile runs inside the container should pick them up if CMake finds Qt6.

If you want, I can add a helper script that clones obs-studio and bootstraps a minimal build inside the container — tell me whether you'd like that and which obs-studio branch/version to target.
