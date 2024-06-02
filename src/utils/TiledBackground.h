#ifndef UI_FRAMEWORK_TILEDBACKGROUND_H
#define UI_FRAMEWORK_TILEDBACKGROUND_H

#include <raylib.h>

class TiledBackground {
public:
    explicit TiledBackground(Texture *texture);

    void Update(float dt);
    void Draw();

    void SetColor(Color c);

private:
    Texture *m_texture;
    Vector2 m_scroll;
    float m_scroll_speed;
    Color m_color;
};


#endif //UI_FRAMEWORK_TILEDBACKGROUND_H
