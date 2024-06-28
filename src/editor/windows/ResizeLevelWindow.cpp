#include "ResizeLevelWindow.h"

#include "engine/widgets/Button.h"
#include "engine/widgets/HLine.h"
#include "engine/widgets/IntField.h"
#include "engine/widgets/Label.h"
#include "engine/widgets/WidgetManager.h"
#include "engine/windows/WindowManager.h"
#include "EditorState.h"

// TODO / FIXME : this is literally a copy-paste from NewLevelWindow

ResizeLevelWindow::ResizeLevelWindow(EditorState *editor) : Window(50, 50, 250, 140) {
    SetTitle("Resize level");
    m_editor = editor;
    m_grid_w = 32;
    m_grid_h = 32;
    m_level_w_m = 32;
    m_level_h_m = 32;

    Label *grid_size_label = new Label(3, 3, 10, "Grid size (tiles) :");
    IntField *grid_w_field = new IntField(153, 3, 95, 10, &m_grid_w);
    IntField *grid_h_field = new IntField(153, 18, 95, 10, &m_grid_w);

    Label *level_size_label = new Label(3, 36, 10, "Level size (m) :");
    IntField *level_w_field = new IntField(153, 36, 95, 10, &m_level_w_m);
    IntField *level_h_field = new IntField(153, 51, 95, 10, &m_level_h_m);

    HLine *line = new HLine(4, 26, 242, WHITE);
    line->SetAlignment(WidgetAlignment_Bottom);

    Button *confirm = new Button(3, 3, 100, 20, "confirm", [this]() {
        m_editor->Resize(m_grid_w, m_grid_h,{(float) m_level_w_m, (float)m_level_h_m});
        GetWindowManager()->CloseWindowByPtr(this);
    });
    confirm->SetAlignment(WidgetAlignment_BottomRight);
    confirm->CenterLabel();

    GetWidgetManager()->AddWidget(grid_size_label);
    GetWidgetManager()->AddWidget(grid_w_field);
    GetWidgetManager()->AddWidget(grid_h_field);
    GetWidgetManager()->AddWidget(level_size_label);
    GetWidgetManager()->AddWidget(level_w_field);
    GetWidgetManager()->AddWidget(level_h_field);
    GetWidgetManager()->AddWidget(line);
    GetWidgetManager()->AddWidget(confirm);
}

ResizeLevelWindow::~ResizeLevelWindow() = default;
