#ifndef A_DETERMINER_COLLECTIBLE_H
#define A_DETERMINER_COLLECTIBLE_H

#include "engine/object/EntityObject.h"

class Player;

class Collectible : public EntityObject {
public:
    Collectible(Vector2 pos, float w, float h);

    void Update(float dt) override;

    /**
     * Should be redefined by subclasses. Can return False to cancel the collecting.
     *  Otherwise, should always return True
     * @param p player
     * @return true if collecting was a success, false otherwise
     */
    virtual bool OnCollect(Player *p) = 0;
};

typedef Collectible* (*CollectibleConstructor)(Vector2);


#endif //A_DETERMINER_COLLECTIBLE_H
