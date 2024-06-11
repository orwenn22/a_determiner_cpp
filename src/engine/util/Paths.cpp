#include "Paths.h"

#if defined(linux) or defined(__EMSCRIPTEN__)

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


std::string RemoveFileFromPath(std::string file_path) {
    int last_slash = -1;
    for(int i = 0; i < file_path.size(); ++i) {
        char c = file_path[i];
        if(c == '/' || c == '\\') last_slash = i;
    }
    if(last_slash == -1) return "";             //no slash in path
    if(last_slash+1 >= file_path.size()) return file_path;
    return file_path.substr(0, last_slash);
}

std::string GetFileFromPath(std::string file_path) {
    int last_slash = -1;
    for(int i = 0; i < file_path.size(); ++i) {
        char c = file_path[i];
        if(c == '/' || c == '\\') last_slash = i;
    }
    if(last_slash <= 0) return file_path;       //no slash in path
    if(last_slash+1 >= file_path.size()) return "";
    return file_path.substr(last_slash+1);
}