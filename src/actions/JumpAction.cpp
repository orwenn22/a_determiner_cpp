#include "JumpAction.h"

#include <cmath>

#include "engine/metrics/Graphics.h"
#include "engine/object/KinematicPrediction.h"
#include "engine/util/VectorOps.h"
#include "GameplayState.h"
#include "GlobalResources.h"
#include "objects/actors/Player.h"
#include "Teams.h"

JumpAction::JumpAction() : Action() {
    m_action_cost = 20;
    m_action_name = "Jump";
    m_is_item = false;
    //TODO : put icon
}

void JumpAction::OnUpdate(Player *player, float dt) {
    //TODO : take into account rebindable controls
    float angle_variation = (float)(IsKeyDown(KEY_D) - IsKeyDown(KEY_A)) * dt;
    player->SetThrowAngle(player->GetThrowAngle() + angle_variation);

    if(IsKeyPressed(KEY_SPACE) && player->GetEnergy() >= m_action_cost) {
        player->SetEnergy(player->GetEnergy() - m_action_cost);

        Vector2 direction = {(float)cos((double)player->GetThrowAngle()), (float)sin((double)player->GetThrowAngle())};
        Vector2 force = direction * player->GetStrength();
        player->ApplyForce(force / dt);     //Dividing by dt is necessary
        player->EnablePhysics();
        player->SetUseSmallHitbox(true);
        player->SetCurrentAction(-1);
        player->GetGameplayState()->HideActionWidgets();
        //TODO : increment jump field in game statistics
    }
}

void JumpAction::OnDraw(Player *player) {
    KinematicPrediction *prediction = new KinematicPrediction(player);

    Vector2 direction = {(float)cos((double)player->GetThrowAngle()), (float)sin((double)player->GetThrowAngle())};
    Vector2 force = direction * player->GetStrength();
    prediction->ApplyForce(force / 0.01f);      // 0.01 is the virtual daltatime
    prediction->DrawSimulation(10);

    player->BlockDefaultSprite();
    float flip_factor = (player->GetThrowAngle() > PI/2 or player->GetThrowAngle() < -PI/2) ? -1.f : 1.f;

    Metrics::DrawSpriteRotEx(Res::player_jump_sprite, {0.f, 0.f, 32.f*flip_factor, 32.f},
                             player->GetPosition(), {player->Width(), player->Height()},
                             0.f, WHITE);
    Metrics::DrawSpriteRotEx(Res::player_jump_sprite, {0.f, 32.f, 32.f*flip_factor, 32.f},
                             player->GetPosition(), {player->Width(), player->Height()},
                             0.f, s_team_colors[player->GetTeam()]);
}
