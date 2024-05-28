#include "Trowel.h"

#include "actions/WallAction.h"
#include "engine/metrics/Graphics.h"
#include "objects/actors/Player.h"
#include "GlobalResources.h"

Trowel::Trowel(Vector2 pos) : Collectible(pos, .5f, .5f) {
    //Nothing
}

Collectible *Trowel::construct(Vector2 pos) {
    return new Trowel(pos);
}

void Trowel::Draw() {
    Metrics::DrawSpriteRotEx(Res::trowel_sprite, {0.f, 0.f, (float)Res::trowel_sprite.width, (float)Res::trowel_sprite.height},
                             m_position,
                             {m_width, m_height},
                             0.f,
                             WHITE);
}

bool Trowel::OnCollect(Player *p) {
    p->AddAction(new WallAction);
    return true;
}
