#include "EditorLayerWindow.h"

#include "editor/widgets/EditorLayerWidget.h"
#include "engine/widgets/WidgetManager.h"

EditorLayerWindow::EditorLayerWindow(EditorState *editor, int x, int y) : Window(x, y, 161, 94+12) {
    SetTitle("Layers");
    m_closable = false;
    GetWidgetManager()->AddWidget(new EditorLayerWidget(editor, 2, 2, 157, 90));
}

EditorLayerWindow::~EditorLayerWindow() = default;
