#include "ActionWidget.h"

#include "actions/Action.h"
#include "objects/actors/Player.h"

ActionWidget::ActionWidget(Player *p, int action_index)
: FakeActionWidget(p->GetAction(action_index)->GetName()) {
    //TODO : null safety ?
    m_player = p;
    m_index = action_index;

    if(p->GetAction(action_index)->IsItem()) {
        m_price_label = "(item)";
    }
    else {
        m_price_label = TextFormat("(-%i)", p->GetAction(action_index)->Cost());
    }

    SetCallback([=]() {
        m_player->GetAction(m_index)->OnClick(m_player, m_index);
    });
}

void ActionWidget::Draw() {
    m_color = (m_index == m_player->GetCurrentAction()) ? RED : WHITE;
    FakeActionWidget::Draw();

    Action *a = m_player->GetAction(m_index);
    if(a == nullptr) return;

    Texture *icon = a->GetIcon();
    if(icon == nullptr) return;

    int x = AbsoluteX();
    int y = AbsoluteY() + m_hovered*m_hover_offset;
    DrawTexturePro(*icon, {0.f, 0.f, (float)icon->width, (float)icon->height},
                   {(float)(x+(Width()-32)/2), (float)(y+(Height()-32)/2), 32.f, 32.f},
                   {0.f, 0.f},
                   0.f,
                   WHITE);
}
