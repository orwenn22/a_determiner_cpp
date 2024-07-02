#ifndef A_DETERMINER_PORTALGUN_H
#define A_DETERMINER_PORTALGUN_H

#include "Collectible.h"

class Portalgun : public Collectible {
public:
    explicit Portalgun(Vector2 pos);
    static Collectible *construct(Vector2 pos);

    bool OnCollect(Player *player) override;
    void Draw() override;
};


#endif //A_DETERMINER_PORTALGUN_H
