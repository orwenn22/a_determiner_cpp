#include "Config.h"

#include "engine/util/Paths.h"
#include "engine/util/Trace.h"

namespace Config {

bool enable_external = false;

std::string GetKeybindConfigPath() {
#if defined(__EMSCRIPTEN__)
    return "/idbfs/.keybinds";      //FIXME : not working
#else
    return GetCWD() + "/.keybinds";
#endif
}


std::string GetExternalFolderPath() {
#if defined(__EMSCRIPTEN__)
    return "/ext";
#else
    return GetCWD() + "/ext";
#endif
}


void InitExtFolder() {
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


};  //namespace Config
