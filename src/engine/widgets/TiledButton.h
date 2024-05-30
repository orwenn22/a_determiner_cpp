#ifndef UI_FRAMEWORK_TILEDBUTTON_H
#define UI_FRAMEWORK_TILEDBUTTON_H

#include "Button.h"

class TiledButton : public Button {
public:
    TiledButton(int x, int y, int w, int h,
                Texture *texture, int length, int factor = 1,
                std::string label = "button",
                std::function<void(void)> callback = DefaultClickableCallback);
    ~TiledButton() override;

    void Draw() override;


protected:
    void DrawBody(Color accent_color);

private:
    Texture *m_texture;
    int m_length;
    int m_factor;
};


#endif //UI_FRAMEWORK_TILEDBUTTON_H
