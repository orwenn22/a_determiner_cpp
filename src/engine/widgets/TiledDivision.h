#ifndef UI_FRAMEWORK_TILEDDIVISION_H
#define UI_FRAMEWORK_TILEDDIVISION_H

#include "Widget.h"

#include <raylib.h>

//TODO : make it possible to set the accent color ?
class TiledDivision : public Widget {
public:
    TiledDivision(int x, int y, int w, int h, Texture *texture, int length, int factor);
    ~TiledDivision() override;

    void Draw() override;

    void SetColor(Color c);

protected:
    void DrawBody(int x_offset, int y_offset, Color accent_color);

    Color m_color;
    Texture *m_texture;
    int m_length;
    int m_factor;
};


#endif //UI_FRAMEWORK_TILEDDIVISION_H
