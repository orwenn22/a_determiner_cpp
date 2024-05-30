#include <cmath>
#include "PortalAction.h"

#include "engine/Globals.h"
#include "engine/metrics/Graphics.h"
#include "engine/metrics/MetricsCamera.h"
#include "engine/object/ObjectManager.h"
#include "objects/actors/Player.h"
#include "objects/actors/Portal.h"
#include "GameplayState.h"
#include "GlobalResources.h"
#include "Teams.h"
#include "Terrain.h"

PortalAction::PortalAction() : Action() {
    m_action_name = "Portal";
    m_is_item = true;
    m_draw_preview = false;
    m_first_portal = nullptr;
    m_icon = &Res::portalgun_sprite;

    m_animation_duration = 1.f;
    m_animation_time = 0.f;
    m_animation_frame_count = 3;
}

void PortalAction::OnUpdate(Player *player, float dt) {
    m_animation_time += dt;
    m_animation_time = (float) fmod((double)m_animation_time, (double)m_animation_duration);

    if(IsMouseUsed()) {
        m_draw_preview = false;
        return;
    }

    m_draw_preview = true;
    if(!IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) return;

    Vector2 mouse_meter = player->GetGameplayState()->GetCamera()->ConvertAbsoluteToMeters(GetMouseX(), GetMouseY());
    Portal *p = new Portal(mouse_meter, player->GetGameplayState());

    auto cols = player->Manager()->GetCollisions(p, 0);
    if(cols.empty() && !player->GetGameplayState()->GetTerrain()->CheckCollisionRec(p->GetRectangle(), true)) {
        player->Manager()->AddObject(p);
        if(m_first_portal == nullptr) {
            m_first_portal = p;
            player->GetGameplayState()->HideActionWidgets();
        }
        else {
            p->SetDestination(m_first_portal);
            m_first_portal->SetDestination(p);
            player->GetGameplayState()->ShowActionWidgets();
            player->RemoveAction(this);
        }
    }
    else {
        delete p;
    }

    UseMouse();
}

void PortalAction::OnDraw(Player *player) {
    player->BlockDefaultSprite();
    int current_frame = (int) (m_animation_time/m_animation_duration * (float)m_animation_frame_count);
    Metrics::DrawSpriteRotEx(Res::player_portal_sprite, {32.f*(float)current_frame, 0.f, 32.f, 47.f},
                             {player->m_position.x, player->m_position.y - .46875f/2.f},
                             {1.f, 1.46875f},
                             0.f,
                             WHITE);
    Metrics::DrawSpriteRotEx(Res::player_portal_sprite, {32.f*(float)current_frame, 47.f, 32.f, 47.f},
                             {player->m_position.x, player->m_position.y - .46875f/2.f},
                             {1.f, 1.46875f},
                             0.f,
                             s_team_colors[player->GetTeam()]);

    // If the mouse is on a UI element then we don't draw the portal preview
    if(!m_draw_preview) return;

    Vector2 mouse_meter = player->GetGameplayState()->GetCamera()->ConvertAbsoluteToMeters(GetMouseX(), GetMouseY());
    Metrics::DrawCircle(mouse_meter, .5f, {150, 92, 191, 127});
}


