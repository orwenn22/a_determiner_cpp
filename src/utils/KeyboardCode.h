#ifndef KEYBOARDCODE_H
#define KEYBOARDCODE_H

#include <functional>
#include <string>

class KeyboardCode {
public:
    KeyboardCode(std::string code, std::function<void(void)> callback);
    ~KeyboardCode();

    void Update();

private:
    std::string m_code;
    std::function<void(void)> m_callback;
    size_t m_index;
    bool m_callback_executed;
};



#endif //KEYBOARDCODE_H
