#include "EditorPaletteWindow.h"

#include "editor/widgets/EditorPaletteWidget.h"
#include "engine/widgets/WidgetManager.h"

EditorPaletteWindow::EditorPaletteWindow(EditorState *editor, int x, int y, int w, int h) : Window(x, y, w, h) {
    SetTitle("Palette");
    m_closable = false;
    GetWidgetManager()->AddWidget(new EditorPaletteWidget(editor, 2, 2, w-4, h-4-12));
}

EditorPaletteWindow::~EditorPaletteWindow() = default;
