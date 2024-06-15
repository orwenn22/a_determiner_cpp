#include "Paths.h"

#include <filesystem>

std::string GetCWD() {
    return std::string(std::filesystem::current_path());
}


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

std::string GetExtensionFromPath(std::string file_path) {
    int last_dot = -1;
    for(int i = 0; i < file_path.size(); ++i) {
        char c = file_path[i];
        if(c == '.') last_dot = i;
    }
    if(last_dot < 0 || file_path.back() == '.') return "";
    return file_path.substr(last_dot+1);
}


bool TryCreateDirectory(std::string path) {
    if(std::filesystem::exists(path)) {
        if(std::filesystem::is_directory(path)) return true;
        else return false;
    }

    if(std::filesystem::create_directory(path)) return true;
    else return false;
}
