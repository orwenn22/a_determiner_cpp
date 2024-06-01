#include <random>
#include <raylib.h>
#include <time.h>

#include "engine/Globals.h"
#include "engine/state/StateManager.h"
#include "engine/util/DebugOverlay.h"
#include "GameplayState.h"
#include "menus/MainMenu.h"
#include "GlobalResources.h"
#include "KeyBinds.h"


int main() {
    srand(time(nullptr));
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(960, 540, "À déterminer");
    SetExitKey(KEY_NULL);
    SetTargetFPS(240);
    KeyBinds::Init();
    Res::LoadAll();

    StateManager *state_manager = new StateManager(new MainMenu);

    bool display_debug_overlay = true;
    while(!WindowShouldClose() && !QuitRequested()) {
        // UPDATE
        float dt = GetFrameTime();
        display_debug_overlay ^= IsKeyPressed(KEY_F3);
        ResetMouse();
        state_manager->Update(dt);

        //DRAW
        BeginDrawing();
        ClearBackground(BLACK);
        state_manager->Draw();
        if(display_debug_overlay) DrawDebugOverlay(dt, state_manager);
        EndDrawing();
    }

    delete state_manager;
    Res::UnloadAll();
    KeyBinds::Finit();
    CloseWindow();
    return 0;
}
