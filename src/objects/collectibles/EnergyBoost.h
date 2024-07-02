#ifndef A_DETERMINER_ENERGYBOOST_H
#define A_DETERMINER_ENERGYBOOST_H

#include "Collectible.h"

class EnergyBoost : public Collectible {
public:
    EnergyBoost(Vector2 position, int points);

    bool OnCollect(Player *p) override;
    void Draw() override;

    static Collectible *construct(Vector2 pos);

private:
    int m_points;
};


#endif //A_DETERMINER_ENERGYBOOST_H
