#ifndef UI_FRAMEWORK_TROWEL_H
#define UI_FRAMEWORK_TROWEL_H

#include "Collectible.h"

class Trowel : public Collectible {
public:
    Trowel(Vector2 pos);
    static Collectible *construct(Vector2 pos);

    void Draw() override;

    bool OnCollect(Player *p) override;
};


#endif //UI_FRAMEWORK_TROWEL_H
