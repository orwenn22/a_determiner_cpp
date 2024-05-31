#include "Globals.h"

#include "util/Trace.h"


static bool s_mouse_used = false;
static bool s_running = true;

bool IsMouseUsed() { return s_mouse_used; }
void UseMouse() {
    if(s_mouse_used) {
        TRACE("UseMouse : mouse already used\n");
    }
    s_mouse_used = true;
}
void ResetMouse() { s_mouse_used = false; }

void RequestQuit() { s_running = false; }
bool QuitRequested() { return !s_running; }
