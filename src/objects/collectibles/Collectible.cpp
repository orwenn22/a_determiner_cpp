#include "Collectible.h"

#include "engine/object/ObjectManager.h"
#include "../ObjectIDs.h"

Collectible::Collectible(Vector2 pos, float w, float h) : EntityObject(pos, w, h) {
    //Nothing for now
}

void Collectible::Update(float dt) {
    auto cols = Manager()->GetCollisions(this, TypeID_Player);
    for(EntityObject *e : cols) {
        if(OnCollect((Player *) e)) {
            Manager()->DestroyObject(this);
            return;
        }
    }
}
