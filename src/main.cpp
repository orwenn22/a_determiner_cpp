#include <raylib.h>
#include <stdio.h>

#include "engine/Globals.h"
#include "engine/state/StateManager.h"
#include "engine/state/State.h"
#include "engine/util/DebugOverlay.h"
#include "GameplayState.h"


int main() {
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(512, 512, "À déterminer");
    SetTargetFPS(240);


    StateManager *state_manager = new StateManager(new GameplayState);

    while(!WindowShouldClose()) {
        // UPDATE
        float dt = GetFrameTime();
        ResetMouse();
        state_manager->Update(dt);

        //DRAW
        BeginDrawing();
        ClearBackground(BLACK);
        state_manager->Draw();
        DrawDebugOverlay(dt, state_manager);
        EndDrawing();

    }

    delete state_manager;
    CloseWindow();
    return 0;
}
