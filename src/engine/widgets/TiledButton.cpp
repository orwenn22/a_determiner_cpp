#include "TiledButton.h"

TiledButton::TiledButton(int x, int y, int w, int h, Texture *texture, int length, int factor, std::string label, std::function<void(void)> callback)
: Button(x, y, w, h, label, callback) {
    m_texture = texture;
    m_factor = factor;
    m_length = length;
}

TiledButton::~TiledButton() = default;

void TiledButton::Draw() {
    if(m_texture == nullptr) {
        Button::Draw();
        return;
    }

    DrawBody((m_hovered) ? m_hover_color : m_color);

    int x = AbsoluteX();
    int y = AbsoluteY() + m_hovered*m_hover_offset;
    DrawText(m_label.c_str(), x+m_label_inner_x, y+m_label_inner_y, m_font_size, WHITE);
}


///////////////////////////////////////
//// PROTECTED

void TiledButton::DrawBody(Color accent_color) {
    float x = (float) AbsoluteX();
    float y = (float) (AbsoluteY() + m_hovered*m_hover_offset);

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
