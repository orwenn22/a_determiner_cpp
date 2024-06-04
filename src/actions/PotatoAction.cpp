#include "PotatoAction.h"

#include "engine/object/ObjectManager.h"
#include "objects/actors/Explosion.h"
#include "objects/actors/Player.h"
#include "objects/ObjectIDs.h"
#include "GameplayState.h"
#include "GlobalResources.h"


PotatoAction::PotatoAction() : Action() {
    m_action_name = "Potato";
    m_is_item = true;
    m_icon = &Res::potato_sprite;
}

void PotatoAction::OnClick(Player *p, int action_index) {
    auto cols = p->Manager()->GetCollisions(p, TypeID_Player);
    for(EntityObject *e : cols) {
        Player *other = (Player *) e;
        if(other->GetTeam() == p->GetTeam()) continue;

        other->AddAction(new PotatoAction);
        if(p->GetEnergy() < 25) p->SetEnergy(25);       //Do this to let the player escape for the next turns
        p->RemoveAction(this);
        return;
    }
}

bool PotatoAction::OnSkip(Player *p) {
    p->Manager()->AddObject(new Explosion(p->m_position, 2.5f, p->GetGameplayState()));
    p->GetGameplayState()->KillPlayer(p);
    return true;        //This will make sure we don't skip two turns (because killing the current player already skip a turn)
}
