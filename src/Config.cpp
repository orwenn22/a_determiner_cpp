#include "Config.h"

#include "engine/util/Paths.h"

namespace Config {

std::string GetKeybindConfigPath() {
#if defined(__EMSCRIPTEN__)
    return "/idbfs/.keybinds";      //FIXME : not working
#else
    return GetCWD() + "/.keybinds";
#endif
}

};
