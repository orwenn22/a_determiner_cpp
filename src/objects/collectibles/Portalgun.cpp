#include "Portalgun.h"

#include "actions/PortalAction.h"
#include "engine/metrics/Graphics.h"
#include "objects/actors/Player.h"
#include "GlobalResources.h"

Portalgun::Portalgun(Vector2 pos) : Collectible(pos, .5f, .5f) {

}

Collectible *Portalgun::construct(Vector2 pos) {
    return new Portalgun(pos);
}

bool Portalgun::OnCollect(Player *player) {
    player->AddAction(new PortalAction);
    return true;
}

void Portalgun::Draw() {
    Metrics::DrawSpriteScale(Res::portalgun_sprite, GetRectangle(), WHITE);
}
