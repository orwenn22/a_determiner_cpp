#ifndef UI_FRAMEWORK_KEYBINDS_H
#define UI_FRAMEWORK_KEYBINDS_H

#include <map>
#include <raylib.h>
#include <string>

enum KeyBindID {
    KeyBind_Left,
    KeyBind_Right,
    KeyBind_Action
};

class KeyBinds {
public:
    // Static stuff
    static void Init();
    static void Finit();
    static void Load(std::string path);
    static inline KeyBinds *The() { return singleton; }

    // Methods
    void Save(std::string path);
    void RegisterKey(KeyBindID keybind_id, KeyboardKey new_keycode);
    bool IsKeyRegistered(KeyBindID keybind_id);
    KeyboardKey GetKeyboardKey(KeyBindID keybind_id);

    bool IsKeyPressed(KeyBindID keybind_id);
    bool IsKeyDown(KeyBindID keybind_id);
    //TODO : key released & up ?
    //TODO 2 : Make it possible to set the state of the key from the outside ? (ex : touch control)

private:
    static KeyBinds *singleton;

    KeyBinds();
    std::map<KeyBindID, KeyboardKey> m_keybinds;
};


#endif //UI_FRAMEWORK_KEYBINDS_H
