#include "ActionWidget.h"

#include "actions/Action.h"
#include "engine/util/Trace.h"
#include "objects/actors/Player.h"


ActionWidget::ActionWidget(Player *p, int action_index)
: FakeActionWidget("Action") {
    m_player = p;
    m_index = action_index;
    if(m_player == nullptr) {
        TRACE("player is null\n");
        return;
    }

    Action *a = p->GetAction(action_index);
    if(a == nullptr) {
        TRACE("action is null\n");
        return;
    }

    m_saved_price = a->Cost();

    if(a->IsItem()) {
        m_price_label = "(item)";
    }
    else {
        m_price_label = TextFormat("(-%i)", a->Cost());
    }

    SetCallback([=]() {
        m_player->GetAction(m_index)->OnClick(m_player, m_index);
    });
}

void ActionWidget::Draw() {
    Action *a = m_player->GetAction(m_index);

    m_color = (m_index == m_player->GetCurrentAction()) ? RED : WHITE;

    if(a != nullptr && a->Cost() != m_saved_price && !a->IsItem()) {
        m_price_label = TextFormat("(-%i)", a->Cost());
    }

    FakeActionWidget::Draw();

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
