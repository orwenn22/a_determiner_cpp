#ifndef A_DETERMINER_TILEDBACKGROUND_H
#define A_DETERMINER_TILEDBACKGROUND_H

#include <raylib.h>

class TiledBackground {
public:
    explicit TiledBackground(Texture *texture);

    void Update(float dt);
    void Draw();

    void SetColor(Color c);
    void SetScroll(Vector2 scroll);
    inline Vector2 GetScroll() { return m_scroll; }

private:
    Texture *m_texture;
    Vector2 m_scroll;
    float m_scroll_speed;
    Color m_color;
};


#endif //A_DETERMINER_TILEDBACKGROUND_H
