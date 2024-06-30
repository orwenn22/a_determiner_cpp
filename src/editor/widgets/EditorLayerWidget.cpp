#include "EditorLayerWidget.h"

#include <raylib.h>

#include "editor/layers/Layer.h"
#include "editor/EditorLevel.h"
#include "engine/Globals.h"
#include "EditorState.h"


//recommended : w = 157 at least
EditorLayerWidget::EditorLayerWidget(EditorState *editor, int x, int y, int w, int h) : Widget(x, y, w, h) {
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
        m_editor->SetCurrentLayerIndex(my / 30);
    }
}

void EditorLayerWidget::Draw() {
    //Widget::Draw();
    int x = AbsoluteX();
    int y = AbsoluteY();

    DrawRectangleLines(x, y, Width(), Height(), WHITE);

    int painter_y = y;
    for(int i = 0; i < m_editor->GetLevel()->GetLayerCount(); ++i) {
        Layer *l = m_editor->GetLevel()->GetLayer(i);
        DrawText(l->Name().c_str(), x+5, painter_y+5, 20, WHITE);
        DrawLine(x, painter_y+30, x+Width(), painter_y+30, WHITE);
        if(i == m_editor->GetCurrentLayerIndex()) {
            DrawRectangleLines(x+2, painter_y+2, Width()-4, 26, YELLOW);
        }
        painter_y += 30;
    }
}
