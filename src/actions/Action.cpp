#include "Action.h"

#include "engine/util/Trace.h"
#include "objects/actors/Player.h"


Action::Action() {
    m_action_name = "default";
    m_action_cost = 0;
    m_is_item = false;
    m_icon = nullptr;
}

Action::~Action() = default;

void Action::OnClick(Player *player, int action_index) {
    player->SetCurrentAction(action_index);
    TRACE("Clicked action %i\n", action_index);
}

bool Action::OnSkip(Player *player) {
    return false;
}

void Action::OnUpdate(Player *player, float dt) {
    //To be overwritten...
}

void Action::OnDraw(Player *player) {
    //To be overwritten...
}
