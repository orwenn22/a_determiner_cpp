#include "FakeActionWidget.h"


#include "GlobalResources.h"

FakeActionWidget::FakeActionWidget(std::string action_name, std::string price_label, std::function<void(void)> callback)
: TiledButton(0, 0, 80, 80, &Res::tiled_button_sprite, 8, 2, action_name, callback){
    m_price_label = price_label;
    SetLabelPosition(6, 6);
    SetFontSize(20);
}

void FakeActionWidget::Draw() {
    TiledButton::Draw();

    int x = AbsoluteX();
    int y = AbsoluteY() + m_hovered*m_hover_offset;

    DrawText(m_price_label.c_str(), x+8, y+Width()-16, 10, GRAY);
}
