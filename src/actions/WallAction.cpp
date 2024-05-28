#include "WallAction.h"

#include <cmath>

#include "engine/metrics/Graphics.h"
#include "engine/object/KinematicPrediction.h"
#include "engine/object/ObjectManager.h"
#include "objects/actors/Player.h"
#include "objects/actors/Wall.h"
#include "GameplayState.h"
#include "GlobalResources.h"
#include "Teams.h"
#include "Terrain.h"


WallAction::WallAction() : Action() {
    m_action_name = "Wall";
    m_is_item = true;
    //TODO : add icon

    m_animation_time = 0.f;
    m_animation_current_frame = 0;
    m_animation_frame_count = 4;
    m_animation_duration = 1.5f;
}

void WallAction::OnClick(Player *player, int action_index) {
    Action::OnClick(player, action_index);
    player->SetThrowAngle(0.f);
}

void WallAction::OnUpdate(Player *player, float dt) {
    // Update character animation
    m_animation_time += dt;
    m_animation_time = (float) fmod((double)m_animation_time, (double)m_animation_duration);
    m_animation_current_frame = (int)((m_animation_time/m_animation_duration) * (float)m_animation_frame_count);

    //TODO : take into account key rebinding
    if(IsKeyPressed(KEY_D)) player->SetThrowAngle(0.f);
    if(IsKeyPressed(KEY_A)) player->SetThrowAngle(PI);
    if(IsKeyPressed(KEY_SPACE))PlaceWall(player);
}

void WallAction::OnDraw(Player *player) {
    Wall w({player->m_position.x + (float)cos(player->GetThrowAngle())*2.f, player->m_position.y},
           .5f, 1.f, player->GetGameplayState());
    KinematicPrediction wall_prediction(&w);
    wall_prediction.DrawSimulation(10);


    player->BlockDefaultSprite();
    Metrics::DrawSpriteRotEx(Res::player_wall_sprite, {32.f*(float)m_animation_current_frame, 0.f, 32.f, 43.f},
                             {player->m_position.x, player->m_position.y - 0.34375f/2},     //Make sure the player sprite is above the ground
                             {1.f, 1.34375f},       //43/32 = 1.34375
                             0.f,
                             WHITE);
    Metrics::DrawSpriteRotEx(Res::player_wall_sprite, {32.f*(float)m_animation_current_frame, 43.f, 32.f, 43.f},
                             {player->m_position.x, player->m_position.y - 0.34375f/2},     //Make sure the player sprite is above the ground
                             {1.f, 1.34375f},       //43/32 = 1.34375
                             0.f,
                             s_team_colors[player->GetTeam()]);
}


//////////////////////////////////////
//// PRIVATE

void WallAction::PlaceWall(Player *player) {
    GameplayState *gs = player->GetGameplayState();
    Wall *w = new Wall({player->m_position.x + (float)cos(player->GetThrowAngle())*2.f, player->m_position.y},
                       .5f, 1.f, player->GetGameplayState());

    // If the wall is clipping with the terrain then make it go up
    float total_vertical_offset = 0.f;
    while(gs->GetTerrain()->CheckCollisionRec(w->GetRectangle())) {
        w->m_position.y -= .1f;         //TODO  :take into account down vector ?
        total_vertical_offset += .1f;
        if(total_vertical_offset > 1.f) {
            delete w;
            return;
        }
    }

    if(!player->Manager()->GetCollisions(w, 0).empty()) {
        delete w;
        return;
    }

    player->Manager()->AddObject(w);
    player->RemoveAction(this);
    //TODO : update game stats
}
