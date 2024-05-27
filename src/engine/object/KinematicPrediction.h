#ifndef UI_FRAMEWORK_KINEMATICPREDICTION_H
#define UI_FRAMEWORK_KINEMATICPREDICTION_H

#include "KinematicObject.h"

class KinematicPrediction : public KinematicObject{
public:
    KinematicPrediction(Vector2 pos, float w, float h, float mass, Vector2 velocity = {0.f, 0.f}, Vector2 acceleration = {0.f, 0.f});

    /**
     * Call the constructor using another object as a base
     * NOTE : if we want to clear the acceleration, this should be done manually after the initialisation of the object
     * @param other the other object we want to "copy"
     */
    explicit KinematicPrediction(KinematicObject *other);

    /**
     * Draw the simulated trajectory
     * @param step if this is 5 this will draw every 5 points of the simulation
     * @param simulation_amount the number of time we want to simulate
     * @param vdt the deltatime we want to simulate (can/should be something else than the ont calculated from FPS, default of 0.01, but can be smaller for higher accuracy)
     * @param c color of the dots
     */
    void DrawSimulation(int step, int simulation_amount = 100, float vdt = 0.01f, Color c = {100, 100, 255, 255});
};


#endif //UI_FRAMEWORK_KINEMATICPREDICTION_H
