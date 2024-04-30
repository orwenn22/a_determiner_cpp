#include <iostream>
#include <raylib.h>

#include "Globals.h"
#include "widgets/Button.h"
#include "widgets/Label.h"
#include "widgets/WidgetManager.h"
#include "windows/WindowManager.h"
#include "windows/Window.h"

int main() {
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(512, 512, "title");
    SetTargetFPS(120);

    WindowManager *window_manager = new WindowManager;
    Window *win = new Window(5, 5, 100, 100);
    window_manager->AddWindow(win);

    Window *win2 = new Window(10, 10, 100, 100);
    window_manager->AddWindow(win2);


    Button *button = new Button(5, 5, 42, 42);
    button->SetAlignment(WidgetAlignment_BottomRight);
    button->SetLabel("wow");
    button->SetCallback([](Clickable *) -> void {
        std::cout << "Hello from custom callback !\n";
    });
    win->GetWidgetManager()->AddWidget(button);

    Label *label = new Label(5, 0, 10, "Hello world !");
    label->SetColor(RED);
    label->SetAlignment(WidgetAlignment_RightCenter);
    win->GetWidgetManager()->AddWidget(label);

    while(!WindowShouldClose()) {
        ResetMouse();
        window_manager->Update();

        BeginDrawing();
        ClearBackground(BLACK);
        window_manager->Draw();
        EndDrawing();
    }

    delete window_manager;
    CloseWindow();
    return 0;
}
