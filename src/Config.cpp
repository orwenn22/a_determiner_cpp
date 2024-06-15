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



};  //namespace Config
