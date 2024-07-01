#include "KeyBinds.h"

#include <cstring>
#include <map>
#include <vector>

#include "util/WasmFS.h"
#include "util/String.h"
#include "util/Trace.h"

KeyBinds *KeyBinds::singleton = nullptr;


///////////////////////////////////////////
//// STATIC

void KeyBinds::Init() {
    if(singleton != nullptr) {
        TRACE("Singleton already initialised\n");
        return;
    }

    singleton = new KeyBinds;
}

void KeyBinds::Finit() {
    if(singleton == nullptr) return;

    delete singleton;
    singleton = nullptr;
}


///////////////////////////////////////
// METHODS

void KeyBinds::Load(std::string path) {
    FILE *in_file = fopen(path.c_str(), "r");
    if(in_file == nullptr) return;

    m_keybinds.clear();     //TODO : maybe we shouldn't clear it ?

    while(!feof(in_file)) {
        char buf[512] = { 0 };
        fgets(buf, 512, in_file);

        RemoveEndOfLine(buf);
        if(strlen(buf) == 0) continue;

        std::vector<std::string> tokens = SplitTokens(buf);
        if(tokens.size() != 2) continue;

        int keybind_id = std::stoi(tokens[0]);
        int key_code = std::stoi(tokens[1]);
        TRACE("Registering %i as %i\n", key_code, keybind_id);
        RegisterKey((KeyBindID) keybind_id, (KeyboardKey) key_code);
    }

    fclose(in_file);
}

void KeyBinds::Save(std::string path) {
    TRACE("Trying to save keybinds to %s\n", path.c_str());
    FILE *out_file = fopen(path.c_str(), "w");
    if(out_file == nullptr) return;

    for(auto pair : m_keybinds) {
        fprintf(out_file, "%i %i\n", pair.first, pair.second);
    }

    fclose(out_file);
#ifdef __EMSCRIPTEN__
    SyncIDBFS();
#endif
    TRACE("Done\n");
}


void KeyBinds::RegisterKey(KeyBindID keybind_id, KeyboardKey new_keycode) {
    TRACE("Registed keycode %i for keybindID %i\n", new_keycode, keybind_id);
    m_keybinds[keybind_id] = new_keycode;
}

bool KeyBinds::IsKeyRegistered(KeyBindID keybind_id) {
    return (m_keybinds.find(keybind_id) != m_keybinds.end());
}

KeyboardKey KeyBinds::GetKeyboardKey(KeyBindID keybind_id) {
    if(m_keybinds.find(keybind_id) == m_keybinds.end()) return (KeyboardKey) 0; //Key not registered
    return m_keybinds[keybind_id];
}


bool KeyBinds::IsKeyPressed(KeyBindID keybind_id) {
    if(m_keybinds.find(keybind_id) == m_keybinds.end()) return false;       //Key not registered
    return ::IsKeyPressed(m_keybinds.at(keybind_id));
}

bool KeyBinds::IsKeyDown(KeyBindID keybind_id) {
    if(m_keybinds.find(keybind_id) == m_keybinds.end()) return false;       //Key not registered
    return ::IsKeyDown(m_keybinds.at(keybind_id));
}


KeyBinds::KeyBinds() = default;
