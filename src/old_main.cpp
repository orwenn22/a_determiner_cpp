#include <iostream>
#include <raylib.h>

#include "engine/Globals.h"
#include "engine/widgets/Button.h"
#include "engine/widgets/HSplit.h"
#include "engine/widgets/IntField.h"
#include "engine/widgets/Label.h"
#include "engine/widgets/VSplit.h"
#include "engine/widgets/WidgetManager.h"
#include "engine/windows/WindowManager.h"
#include "engine/windows/Window.h"

int main() {
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(512, 512, "title");
    SetTargetFPS(120);

    int some_value = 42;

    WidgetManager *widget_manager = new WidgetManager;

    WindowManager *window_manager = new WindowManager;
    Window *win = new Window(5, 5, 100, 100);
    window_manager->AddWindow(win);

    Window *win2 = new Window(10, 10, 200, 200);
    window_manager->AddWindow(win2);

    HSplit *win2_hsplit = new HSplit;
    win2->GetWidgetManager()->AddWidget(win2_hsplit);

    Label *l_win2_l = new Label(0, 0, 10, "LEFT");
    l_win2_l->SetAlignment(WidgetAlignment_Center);
    win2_hsplit->Left()->AddWidget(l_win2_l);

    Label *l_win2_r = new Label(0, 0, 10, "RIGHT");
    l_win2_r->SetAlignment(WidgetAlignment_Center);
    win2_hsplit->Right()->AddWidget(l_win2_r);


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

    IntField *int_field = new IntField(1, 1, 75, 10, &some_value);
    int_field->SetMin(12);
    int_field->SetMax(542);
    win->GetWidgetManager()->AddWidget(int_field);


    HSplit *hsplit = new HSplit;
    widget_manager->AddWidget(hsplit);


    Button *button2 = new Button(0, 0, 42, 42, "Wow", [](){std::cout << "hello\n";});
    //button2->OccupyAllSpace(10, 10, 10, 10);
    button2->SetAlignment(WidgetAlignment_Center);
    hsplit->Left()->AddWidget(button2);

    VSplit *vsplit = new VSplit;
    hsplit->Right()->AddWidget(vsplit);

    Label *ltop = new Label(0, 0, 20, "TOP");
    ltop->SetAlignment(WidgetAlignment_Center);
    vsplit->Top()->AddWidget(ltop);

    Label *lbottom = new Label(0, 0, 20, "BOTTOM");
    lbottom->SetAlignment(WidgetAlignment_Center);
    vsplit->Bottom()->AddWidget(lbottom);



    while(!WindowShouldClose()) {
        ResetMouse();
        window_manager->Update();
        widget_manager->Update();

        BeginDrawing();
        ClearBackground(BLACK);
        widget_manager->Draw();
        window_manager->Draw();
        DrawFPS(2, 2);
        EndDrawing();
    }

    delete window_manager;
    delete widget_manager;
    CloseWindow();
    return 0;
}
