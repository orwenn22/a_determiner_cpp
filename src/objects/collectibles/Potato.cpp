#include "Potato.h"

#include "actions/PotatoAction.h"
#include "engine/metrics/Graphics.h"
#include "objects/actors/Player.h"
#include "GlobalResources.h"


Potato::Potato(Vector2 pos) : Collectible(pos, .5f, .5f) {
    //TODO : setup animation ?
}

Collectible *Potato::construct(Vector2 pos) {
    return new Potato(pos);
}


bool Potato::OnCollect(Player *p) {
    //TODO : check if the player already have a potato ?
    p->AddAction(new PotatoAction);
    return true;
}

void Potato::Draw() {
    Metrics::DrawSpriteRot(Res::potato_sprite, m_position, {Width(), Height()}, 0.f, WHITE);
}
