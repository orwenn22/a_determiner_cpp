#include "TiledDivision.h"

TiledDivision::TiledDivision(int x, int y, int w, int h, Texture *texture, int length, int factor) : Widget(x, y, w, h) {
    m_color = WHITE;
    m_texture = texture;
    m_factor = factor;
    m_length = length;
}

TiledDivision::~TiledDivision() {

}

void TiledDivision::Draw() {
    DrawBody(0, 0, m_color);
}

void TiledDivision::SetColor(Color c) {
    m_color = c;
}


//////////////////////////////:
//// PROTECTED

void TiledDivision::DrawBody(int x_offset, int y_offset, Color accent_color) {
    float x = (float) (AbsoluteX() + x_offset);
    float y = (float) (AbsoluteY() + y_offset);

    float flength = (float)m_length;
    float real_length = flength * (float)m_factor;

    float center_x_origin = x+real_length;
    float center_x_size = (float)Width() - real_length*2.f;
    float right_x_origin = center_x_origin + center_x_size;

    float center_y_origin = y+real_length;
    float center_y_size = (float)Height() - real_length*2.f;
    float bottom_y_origin = center_y_origin + center_y_size;

    //TOP LEFT
    DrawTexturePro(*m_texture, {0.f, 0.f, flength, flength},
                   {x, y, real_length, real_length},
                   {0.f, 0.f}, 0.f, accent_color);

    //TOP CENTER
    DrawTexturePro(*m_texture, {flength, 0.f, flength, flength},
                   {center_x_origin, y, center_x_size, real_length},
                   {0.f, 0.f}, 0.f, accent_color);

    //TOP RIGHT
    DrawTexturePro(*m_texture, {flength*2.f, 0.f, flength, flength},
                   {right_x_origin , y, real_length, real_length},
                   {0.f, 0.f}, 0.f, accent_color);

    //MIDDLE LEFT
    DrawTexturePro(*m_texture, {0.f, flength, flength, flength},
                   {x, center_y_origin, real_length, center_y_size},
                   {0.f, 0.f}, 0.f, accent_color);

    //MIDDLE CENTER
    DrawTexturePro(*m_texture, {flength, flength, flength, flength},
                   {center_x_origin, center_y_origin, center_x_size, center_y_size},
                   {0.f, 0.f}, 0.f, accent_color);

    //MIDDLE RIGHT
    DrawTexturePro(*m_texture, {flength*2.f, flength, flength, flength},
                   {right_x_origin, center_y_origin, real_length, center_y_size},
                   {0.f, 0.f}, 0.f, accent_color);

    //BOTTOM LEFT
    DrawTexturePro(*m_texture, {0.f, flength*2, flength, flength},
                   {x, bottom_y_origin, real_length, real_length},
                   {0.f, 0.f}, 0.f, accent_color);

    //BOTTOM CENTER
    DrawTexturePro(*m_texture, {flength, flength*2, flength, flength},
                   {center_x_origin, bottom_y_origin, center_x_size, real_length},
                   {0.f, 0.f}, 0.f, accent_color);

    //BOTTOM RIGHT
    DrawTexturePro(*m_texture, {flength*2, flength*2, flength, flength},
                   {right_x_origin, bottom_y_origin, real_length, real_length},
                   {0.f, 0.f}, 0.f, accent_color);
}
