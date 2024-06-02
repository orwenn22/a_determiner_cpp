#include "StrengthModifier.h"

#include "engine/metrics/Graphics.h"
#include "objects/actors/Player.h"
#include "GlobalResources.h"


StrengthModifier::StrengthModifier(Vector2 pos, int points) : Collectible(pos, .5f, .5f) {
    m_points = points;
}

StrengthModifier::~StrengthModifier() {

}

bool StrengthModifier::OnCollect(Player *p) {
    if(p->GetStrength() + (float)m_points <= 0) return false;
    p->SetStrength(p->GetStrength() + (float)m_points);
    return true;
}

void StrengthModifier::Update(float dt) {
    //TODO : floating animation
    Collectible::Update(dt);
}

void StrengthModifier::Draw() {
    Texture *texture = (m_points < 0) ? &Res::strength_downgrade_sprite : &Res::strength_upgrade_sprite;
    Metrics::DrawSpriteRot(*texture, m_position, {m_width, m_height}, 0.f, WHITE);
}

Collectible *StrengthModifier::construct_upgrade(Vector2 pos) {
    return new StrengthModifier(pos, 10);
}

Collectible *StrengthModifier::construct_downgrade(Vector2 pos) {
    return new StrengthModifier(pos, -10);
}
