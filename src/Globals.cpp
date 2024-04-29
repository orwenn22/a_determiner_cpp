#include "Globals.h"

#include <iostream>

static bool s_mouse_used = false;

bool IsMouseUsed() { return s_mouse_used; }
void UseMouse() {
    if(s_mouse_used) std::cout << "UseMouse : mouse already used\n";
    s_mouse_used = true;
}
void ResetMouse() { s_mouse_used = false; }
