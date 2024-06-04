#include "Open.h"

#include <string>

#include "Trace.h"


#if defined(__linux__)

#include <cstdlib>

int OpenWithDefault(const char *path) {
    std::string command = "xdg-open ";
    command += path;
    return system(command.c_str());
}

#elif defined(_WIN32)

#include <windows.h>

int OpenWithDefault(const char *path) {
    HINSTANCE ret = ShellExecute(0, 0, path 0, 0 , SW_SHOW);
    if(ret > 32) return 0;
    return (int) ret;
}

#elif defined(__APPLE__)

#include <cstdlib>

int OpenWithDefault(const char *path) {
    std::string command = "open ";
    command += path;
    return system(command.c_str());
}

#else

int OpenWithDefault(const char *path) {
    TRACE("Not implemented for current platform :(\n");
}

#endif