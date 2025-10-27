// Simple no-op implementations for missing obs-frontend API functions.
// These allow building the plugin against older obs-frontend-api headers.
#include "obs_frontend_compat.h"

extern "C" {
void obs_frontend_take_screenshot(void)
{
    // no-op fallback
}

void obs_frontend_take_source_screenshot(obs_source_t * /*source*/)
{
    // no-op fallback
}

void obs_frontend_set_tbar_position(int /*pos*/)
{
    // no-op fallback
}

void obs_frontend_release_tbar(void)
{
    // no-op fallback
}
}
