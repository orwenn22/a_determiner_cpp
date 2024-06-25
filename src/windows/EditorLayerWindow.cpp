#include "EditorLayerWindow.h"

#include "engine/widgets/WidgetManager.h"
#include "widgets/EditorLayerWidget.h"

EditorLayerWindow::EditorLayerWindow(EditorState *editor, int x, int y) : Window(x, y, 161, 94+12) {
    SetTitle("Layers");
    m_closable = false;
    GetWidgetManager()->AddWidget(new EditorLayerWidget(editor, 2, 2));
}

EditorLayerWindow::~EditorLayerWindow() = default;
