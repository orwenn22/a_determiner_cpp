#ifndef UI_FRAMEWORK_POTATO_H
#define UI_FRAMEWORK_POTATO_H

#include "Collectible.h"

class Potato : public Collectible {
public:
    explicit Potato(Vector2 pos);
    static Collectible *construct(Vector2 pos);

    bool OnCollect(Player *p) override;
    void Draw() override;
};


#endif //UI_FRAMEWORK_POTATO_H
