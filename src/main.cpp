#include <iostream>
#include <raylib.h>

#include "Globals.h"
#include "widgets/Button.h"
#include "widgets/WidgetManager.h"

int main() {
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(512, 512, "title");

    WidgetManager *wm = new WidgetManager;
    Button *button = new Button(5, 5, 42, 42);
    button->SetAlignment(WidgetAlignment_BottomRight);
    button->SetLabel("wow");
    button->SetCallback([](Button *) -> void {
        std::cout << "Hello from custom callback !\n";
    });


    wm->AddWidget(button);

    while(!WindowShouldClose()) {
        ResetMouse();
        wm->Update();

        BeginDrawing();
        ClearBackground(BLACK);
        wm->Draw();
        EndDrawing();
    }

    delete wm;
    CloseWindow();
    return 0;
}
