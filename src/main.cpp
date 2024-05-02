#include <iostream>
#include <raylib.h>

#include "Globals.h"
#include "widgets/Button.h"
#include "widgets/HSplit.h"
#include "widgets/Label.h"
#include "widgets/WidgetManager.h"
#include "windows/WindowManager.h"
#include "windows/Window.h"

int main() {
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(512, 512, "title");
    SetTargetFPS(120);

    WidgetManager *widget_manager = new WidgetManager;

    WindowManager *window_manager = new WindowManager;
    Window *win = new Window(5, 5, 100, 100);
    window_manager->AddWindow(win);

    Window *win2 = new Window(10, 10, 100, 100);
    window_manager->AddWindow(win2);


    Button *button = new Button(5, 5, 42, 42);
    button->SetAlignment(WidgetAlignment_BottomRight);
    button->SetLabel("wow");
    button->SetCallback([]() -> void {
        std::cout << "Hello from custom callback !\n";
    });
    win->GetWidgetManager()->AddWidget(button);

    Label *label = new Label(5, 0, 10, "Hello world !");
    label->SetColor(RED);
    label->SetAlignment(WidgetAlignment_RightCenter);
    win->GetWidgetManager()->AddWidget(label);


    HSplit *hsplit = new HSplit;
    widget_manager->AddWidget(hsplit);


    Button *button2 = new Button(0, 0, 42, 42, "Wow", [](){std::cout << "hello\n";});
    button2->OccupyAllSpace(10, 10, 10, 10);
    hsplit->Left()->AddWidget(button2);

    Button *button3 = new Button(0, 0, 42, 42, "Wow", [](){std::cout << "hello\n";});
    button3->OccupyAllSpace(10, 10, 10, 10);
    hsplit->Right()->AddWidget(button3);



    while(!WindowShouldClose()) {
        ResetMouse();
        window_manager->Update();
        widget_manager->Update();

        BeginDrawing();
        ClearBackground(BLACK);
        widget_manager->Draw();
        window_manager->Draw();
        EndDrawing();
    }

    delete window_manager;
    delete widget_manager;
    CloseWindow();
    return 0;
}
