#include "EnergyBoost.h"

#include "engine/metrics/Graphics.h"
#include "objects/actors/Player.h"
#include "GlobalResources.h"

EnergyBoost::EnergyBoost(Vector2 position, int points) : Collectible(position, .5f, .5f) {
    m_points = points;
    if(m_points <= 0) m_points = 1;
}

bool EnergyBoost::OnCollect(Player *p) {
    p->SetEnergy(p->GetEnergy() + m_points);
    return true;
}

void EnergyBoost::Draw() {
    //TODO : floating animation
    Metrics::DrawSpriteRot(Res::battery_sprite, m_position, {m_width, m_height}, .0f, WHITE);
}

Collectible *EnergyBoost::construct(Vector2 pos) {
    return new EnergyBoost(pos, 20);
}
