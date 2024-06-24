#include "EditorLayerWidget.h"

#include <raylib.h>

#include "engine/Globals.h"
#include "EditorState.h"


EditorLayerWidget::EditorLayerWidget(EditorState *editor, int x, int y) : Widget(x, y, 150, 60) {
    m_editor = editor;
}

EditorLayerWidget::~EditorLayerWidget() = default;

void EditorLayerWidget::Update() {
    //Widget::Update();
    if(!IsMouseHovering() || IsMouseUsed()) return;
    UseMouse();

    if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        //int mx = GetMouseX() - AbsoluteX();
        int my = GetMouseY() - AbsoluteY();
        m_editor->SetEditingCollisions(my < 30);
    }
}

void EditorLayerWidget::Draw() {
    //Widget::Draw();
    int x = AbsoluteX();
    int y = AbsoluteY();

    DrawRectangleLines(x, y, Width(), Height(), WHITE);
    DrawLine(x, y+30, x+Width(), y+30, WHITE);
    DrawRectangleLines(x+2, y+2 + (!m_editor->IsEditingCollisions())*30, Width()-4, 26, YELLOW);
    DrawText("Collisions", x+5, y+5, 20, WHITE);
    DrawText("Tilemap", x+5, y+35, 20, WHITE);
}
