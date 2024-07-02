#ifndef A_DETERMINER_TROWEL_H
#define A_DETERMINER_TROWEL_H

#include "Collectible.h"

class Trowel : public Collectible {
public:
    Trowel(Vector2 pos);
    static Collectible *construct(Vector2 pos);

    void Draw() override;

    bool OnCollect(Player *p) override;
};


#endif //A_DETERMINER_TROWEL_H
