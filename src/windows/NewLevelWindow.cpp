#include "NewLevelWindow.h"

#include "engine/widgets/Button.h"
#include "engine/widgets/HLine.h"
#include "engine/widgets/IntField.h"
#include "engine/widgets/Label.h"
#include "engine/widgets/WidgetManager.h"
#include "EditorState.h"


NewLevelWindow::NewLevelWindow(EditorState *editor) : Window(50, 50, 250, 140) {
    SetTitle("New level");
    m_editor = editor;
    m_grid_w = 32;
    m_grid_h = 32;
    m_level_w_m = 32;
    m_level_h_m = 32;
    m_tile_w_px = 16;
    m_tile_h_px = 16;

    Label *grid_size_label = new Label(3, 3, 10, "Grid size (tiles) :");
    IntField *grid_w_field = new IntField(153, 3, 95, 10, &m_grid_w);
    IntField *grid_h_field = new IntField(153, 18, 95, 10, &m_grid_w);

    Label *level_size_label = new Label(3, 36, 10, "Level size (m) :");
    IntField *level_w_field = new IntField(153, 36, 95, 10, &m_level_w_m);
    IntField *level_h_field = new IntField(153, 51, 95, 10, &m_level_h_m);

    Label *tile_size_label = new Label(3, 69, 10, "Tile size on tileset (px) :");
    IntField *tile_w_field = new IntField(153, 69, 95, 10, &m_tile_w_px);
    IntField *tile_h_field = new IntField(153, 84, 95, 10, &m_tile_h_px);

    HLine *line = new HLine(4, 26, 242, WHITE);
    line->SetAlignment(WidgetAlignment_Bottom);

    Button *confirm = new Button(3, 3, 100, 20, "confirm", [this]() {
        m_editor->CreateNew(m_grid_w, m_grid_h, m_tile_w_px, m_tile_h_px, {(float) m_level_w_m, (float)m_level_h_m});
    });
    confirm->SetAlignment(WidgetAlignment_BottomRight);
    confirm->CenterLabel();

    GetWidgetManager()->AddWidget(grid_size_label);
    GetWidgetManager()->AddWidget(grid_w_field);
    GetWidgetManager()->AddWidget(grid_h_field);
    GetWidgetManager()->AddWidget(level_size_label);
    GetWidgetManager()->AddWidget(level_w_field);
    GetWidgetManager()->AddWidget(level_h_field);
    GetWidgetManager()->AddWidget(tile_size_label);
    GetWidgetManager()->AddWidget(tile_w_field);
    GetWidgetManager()->AddWidget(tile_h_field);
    GetWidgetManager()->AddWidget(line);
    GetWidgetManager()->AddWidget(confirm);
}

NewLevelWindow::~NewLevelWindow() = default;
