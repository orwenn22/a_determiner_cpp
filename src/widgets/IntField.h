#ifndef UI_FRAMEWORK_INTFIELD_H
#define UI_FRAMEWORK_INTFIELD_H

#include "Widget.h"

#include <optional>

class IntField : public Widget {
public:
    IntField(int x, int y, int w, int font_size, int *target);
    ~IntField() override;

    void Update() override;
    void Draw() override;

    void SetMin(int value);
    void SetMax(int value);

private:
    void ProcessKeyboardInput();

    bool m_selected;
    int m_font_size;
    int *m_target;
    int m_displayed_value;

    std::optional<int> m_min;
    std::optional<int> m_max;
};


#endif //UI_FRAMEWORK_INTFIELD_H
