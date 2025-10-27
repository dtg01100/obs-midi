// Compatibility shims for missing obs-frontend-api declarations
#pragma once

#include <obs-frontend-api.h>

// Provide fallback enum value if the system header is older.
#ifndef OBS_FRONTEND_EVENT_REPLAY_BUFFER_SAVED
#define OBS_FRONTEND_EVENT_REPLAY_BUFFER_SAVED 9999
#endif

// Declare functions that may be missing from older obs-frontend-api headers.
// Provide implementations in obs_frontend_compat.cpp so the project can compile
// against older headers. These implementations are no-ops.
extern "C" {
void obs_frontend_take_screenshot(void);
void obs_frontend_take_source_screenshot(obs_source_t *source);
void obs_frontend_set_tbar_position(int pos);
void obs_frontend_release_tbar(void);
}
