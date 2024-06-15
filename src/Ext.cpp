#include "Ext.h"

#include <filesystem>

#include "engine/util/Paths.h"
#include "engine/util/Trace.h"
#include "Config.h"


namespace Ext {

void Init() {
    std::string ext_path = Config::GetExternalFolderPath();
    if(ext_path.empty()) {
        Config::enable_external = false;
        return;
    }

    //ext directory
    if(!TryCreateDirectory(ext_path)) {
        TRACE("Failed to init ext directory at %s\n", ext_path.c_str());
        Config::enable_external = false;
        return;
    }

    //ext/maps directory
    if(!TryCreateDirectory(ext_path + "/maps")) {
        TRACE("Failed to init ext/maps directory at %s\n", ext_path.c_str());
        Config::enable_external = false;
        return;
    }

    //ext/res directory
    if(!TryCreateDirectory(ext_path + "/res")) {
        TRACE("Failed to init ext/res directory at %s\n", ext_path.c_str());
        Config::enable_external = false;
        return;
    }

    Config::enable_external = true;
    TRACE("Successfully enabled ext directory at %s\n", ext_path.c_str());
}


/**
 * Check if a path exists as a relative/absolute path, then check in the ext folder (if Config::enable_external is true)
 * @param path the path we want to check
 * @return the corresponding path if it exists, an empty string if not
 */
std::string ResolvePath(std::string path) {
    //Relative or absolute path
    if(std::filesystem::exists(path)) return path;

    if(!Config::enable_external) return "";

    //In ext directory
    std::filesystem::path ext_path = Config::GetExternalFolderPath() + "/" + path;
    if(std::filesystem::exists(ext_path)) return ext_path.string();
    return "";
}

};  //namespace Ext
