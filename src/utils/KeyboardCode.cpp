#include "KeyboardCode.h"

#include <raylib.h>


KeyboardCode::KeyboardCode(std::string code, std::function<void(void)> callback) {
    m_code = code;
    m_callback = callback;
    m_index = 0;
    m_callback_executed = false;
}

KeyboardCode::~KeyboardCode() = default;

void KeyboardCode::Update() {
    if(m_callback_executed) return;

    for(int key = KEY_SPACE; key <= KEY_Z; ++key) {
        if(!IsKeyPressed(key) || m_index >= m_code.size()) continue;
        if(m_code[m_index] == key) ++m_index;
        else m_index = 0;
    }

    if(m_index == m_code.size()) {
        m_callback();
        m_callback_executed = true;
    }
}
