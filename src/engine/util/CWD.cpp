#include "CWD.h"

#if defined(linux)

#include <unistd.h>

std::string GetCWD() {
    char buf[512];
    getcwd(buf, 512);
    return std::string(buf);
}

#elif defined(_WIN32)
//TODO : test windows implementation

#include <windows.h>

std::string GetCWD() {
    char buf[512];
    (void) GetCurrentDirectory(512, buf);
    return std::string(buf);
}

#else

std::string GetCWD() {
    return "";
}

#endif
