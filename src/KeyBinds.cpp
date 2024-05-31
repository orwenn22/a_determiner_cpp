#include "KeyBinds.h"

#include <map>

#include "engine/util/Trace.h"

KeyBinds *KeyBinds::singleton = nullptr;

void KeyBinds::Init() {
    if(singleton != nullptr) {
        TRACE("Singleton already initialised\n");
        return;
    }
    //TODO : check if configuration exist, then load it

    //If no config exist then create one from defaults
    singleton = new KeyBinds;
    singleton->RegisterKey(KeyBind_Action, KEY_SPACE);
    singleton->RegisterKey(KeyBind_Left, KEY_A);
    singleton->RegisterKey(KeyBind_Right, KEY_D);
}

void KeyBinds::Finit() {
    //TODO : save configuration

    delete singleton;
    singleton = nullptr;
}


void KeyBinds::RegisterKey(KeyBindID keybind_id, KeyboardKey new_keycode) {
    TRACE("Registed keycode %i for keybindID %i\n", new_keycode, keybind_id);
    m_keybinds[keybind_id] = new_keycode;
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
