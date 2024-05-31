#ifndef UI_FRAMEWORK_KEYBINDS_H
#define UI_FRAMEWORK_KEYBINDS_H

#include <map>
#include <raylib.h>

enum KeyBindID {
    KeyBind_Left,
    KeyBind_Right,
    KeyBind_Action
};

class KeyBinds {
public:
    static void Init();
    static void Finit();
    static inline KeyBinds *The() { return singleton; }

    void RegisterKey(KeyBindID keybind_id, KeyboardKey new_keycode);
    KeyboardKey GetKeyboardKey(KeyBindID keybind_id);

    bool IsKeyPressed(KeyBindID keybind_id);
    bool IsKeyDown(KeyBindID keybind_id);
    //TODO : key released & up ?

private:
    static KeyBinds *singleton;

    KeyBinds();
    std::map<KeyBindID, KeyboardKey> m_keybinds;
};


#endif //UI_FRAMEWORK_KEYBINDS_H
