#ifndef A_DETERMINER_WALL_H
#define A_DETERMINER_WALL_H

#include "engine/object/KinematicObject.h"
#include <vector>

class GameplayState;

class Wall : public KinematicObject {
public:
    Wall(Vector2 pos, float w, float h, GameplayState *gameplay_state);

    void Update(float dt) override;
    void Draw() override;

    bool Grounded();
    bool CollideWithSolidObject();

private:
    void UpdatePhysics(float dt);

    GameplayState *m_gameplay_state;
    std::vector<int> m_solid_types;
};


#endif //A_DETERMINER_WALL_H
