
#include "ShootAction.h"

#include <cmath>

#include "engine/metrics/Graphics.h"
#include "engine/object/KinematicPrediction.h"
#include "engine/object/ObjectManager.h"
#include "engine/util/VectorOps.h"
#include "objects/actors/Bullet.h"
#include "objects/actors/Player.h"
#include "GlobalResources.h"
#include "KeyBinds.h"
#include "Teams.h"


ShootAction::ShootAction() : Action() {
    m_action_cost = 25;
    m_action_name = "Shoot";
    m_icon = &Res::shoot_icon_sprite;
}

void ShootAction::OnUpdate(Player *player, float dt) {
    KeyBinds *kb = KeyBinds::The();
    float angle_variation = (float)(kb->IsKeyDown(KeyBind_Right) - kb->IsKeyDown(KeyBind_Left)) * dt;
    player->SetThrowAngle(player->GetThrowAngle() + angle_variation);

    if(kb->IsKeyPressed(KeyBind_Action) && player->GetEnergy() >= m_action_cost) {
        Bullet *b = new Bullet(player->GetPosition(), player->GetGameplayState(), player, true);

        Vector2 direction = {(float)cos((double)player->GetThrowAngle()), (float)sin((double)player->GetThrowAngle())};
        Vector2 force = direction * player->GetStrength();
        b->ApplyForce(force / dt);      //Dividing by dt is necessary to bypass the *dt of the acceleration

        player->Manager()->AddObject(b);
        player->SetEnergy(player->GetEnergy() - m_action_cost);
        //TODO : increment shoot field in game statistics
    }
}

void ShootAction::OnDraw(Player *player) {
    Bullet b(player->GetPosition(), player->GetGameplayState(), player, true);
    KinematicPrediction a(&b);

    Vector2 direction = {(float)cos((double)player->GetThrowAngle()), (float)sin((double)player->GetThrowAngle())};
    Vector2 force = direction * player->GetStrength();
    a.ApplyForce(force / 0.01f);        // 0.01 is the virtual daltatime
    a.DrawSimulation(10);

    player->BlockDefaultSprite();
    Metrics::DrawSpriteRotEx(Res::player_shooting_sprite, {0.f, 0.f, 32.f, 32.f},
                             player->GetPosition(),
                             {1.f, 1.f},
                             0.f,
                             WHITE);
    Metrics::DrawSpriteRotEx(Res::player_shooting_sprite, {0.f, 32.f, 32.f, 32.f},
                             player->GetPosition(),
                             {1.f, 1.f},
                             0.f,
                             s_team_colors[player->GetTeam()]);
}


