#ifndef A_DETERMINER_STRENGTHMODIFIER_H
#define A_DETERMINER_STRENGTHMODIFIER_H

#include "Collectible.h"

class StrengthModifier : public Collectible {
public:
    StrengthModifier(Vector2 pos, int points);
    ~StrengthModifier() override;

    bool OnCollect(Player *p) override;

    void Update(float dt) override;
    void Draw() override;

    static Collectible *construct_upgrade(Vector2 pos);
    static Collectible *construct_downgrade(Vector2 pos);

private:
    int m_points;
};


#endif //A_DETERMINER_STRENGTHMODIFIER_H
