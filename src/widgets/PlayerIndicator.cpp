#include "PlayerIndicator.h"

#include "actions/Action.h"
#include "engine/Globals.h"
#include "engine/metrics/MetricsCamera.h"
#include "objects/actors/Player.h"
#include "GameplayState.h"
#include "GlobalResources.h"
#include "Teams.h"


PlayerIndicator::PlayerIndicator(GameplayState *gameplay_state) : Widget(0, 10, 1, 32) {
    m_gameplay_state = gameplay_state;
    SetAlignment(WidgetAlignment_HCenter);
}

PlayerIndicator::~PlayerIndicator() = default;

void PlayerIndicator::Update() {
    SetWidth(32*m_gameplay_state->GetPlayerCount());

    if(IsMouseUsed() || !IsMouseHovering()) return;

    int relative_mouse_x = GetMouseX() - AbsoluteX();
    int player_index = relative_mouse_x / 32;
    Player *p = m_gameplay_state->GetPlayer(player_index);
    //TODO : give tooltip
    if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        if(p != nullptr) m_gameplay_state->GetCamera()->SetCameraCenter(p->m_position);
    }

    UseMouse();
}

void PlayerIndicator::Draw() {
    int painter_x = AbsoluteX();
    int painter_y = AbsoluteY();

    int i_stop = m_gameplay_state->GetPlayerCount();
    for(int i = 0; i < i_stop; ++i) {
        Rectangle dest = {(float)painter_x, (float)painter_y, 32.f, 32.f};
        Player *p = m_gameplay_state->GetPlayer(i);


        if(p == nullptr) {
            DrawTexturePro(Res::player_mini_ded_sprite, {0.f, 0.f, 16.f, 16.f},
                           dest,
                           {0.f, 0.f}, 0.f,
                           WHITE);

            painter_x += 32;
            continue;
        }

        DrawTexturePro(Res::player_mini_sprite, {0.f, 0.f, 16.f, 16.f},
                       dest,
                       {0.f, 0.f}, 0.f,
                       WHITE);
        DrawTexturePro(Res::player_mini_sprite, {0.f, 16.f, 16.f, 16.f},
                       dest,
                       {0.f, 0.f}, 0.f,
                       s_team_colors[p->GetTeam()]);
        dest.y += 32.f;

        //If we get here then p is not dead

        //Items
        int j_stop = p->GetActionCount();
        for(int j = 0; j < j_stop; ++j) {
            Action *act = p->GetAction(j);
            if(act == nullptr) continue;
            Texture *icon = act->GetIcon();
            if(!act->IsItem() || icon == nullptr) continue;

            DrawTexturePro(*icon, {0.f, 0.f, (float)icon->width, (float)icon->height},
                           dest,
                           {0.f, 0.f}, 0.f,
                           WHITE);

            dest.y += 32.f;
        }

        //Green marker
        if(p == m_gameplay_state->GetCurrentPlayer()) {
            DrawTexturePro(Res::green_marker_sprite, {0.f, 0.f, 50.f, -50.f},
                           dest,
                           {0.f, 0.f}, 0.f,
                           WHITE);

            dest.y += 32.f;
        }

        painter_x += 32;
    }
}
