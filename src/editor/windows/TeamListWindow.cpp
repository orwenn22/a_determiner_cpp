#include "TeamListWindow.h"

#include "editor/layers/LayerSpawnRegions.h"
#include "editor/widgets/TeamListWidget.h"
#include "editor/EditorState.h"
#include "engine/widgets/Button.h"
#include "engine/widgets/WidgetManager.h"
#include "Teams.h"


TeamListWindow::TeamListWindow(EditorState *editor, LayerSpawnRegions *spawn_regions_layer, int x, int y) : Window(x, y, 200, 200) {
    m_closable = false;
    SetTitle("Teams");

    m_editor = editor;
    m_spawn_regions_layer = spawn_regions_layer;

    GetWidgetManager()->AddWidget(new TeamListWidget(editor, spawn_regions_layer, 3, 3, 200-6, 200-6-12-18));

    Button *add_team = new Button(3, 3, 90, 12, "Add team", [this]() {
        int curreent_team_count = m_spawn_regions_layer->GetSpawnRegionCount();
        if(curreent_team_count >= s_team_count) return;
        m_spawn_regions_layer->AddSpawnRegion(EditorSpawnRegion(m_spawn_regions_layer->Level(), 2.f, 2.f, 5.f, 5.f, curreent_team_count));
        m_editor->SetCurrentLayer(m_spawn_regions_layer);
    });
    add_team->CenterLabel();
    add_team->SetAlignment(WidgetAlignment_Bottom);

    Button *remove_team = new Button(3, 3, 90, 12, "Remove team", [this]() {
        int curreent_team_count = m_spawn_regions_layer->GetSpawnRegionCount();
        if(curreent_team_count <= 2) return;
        m_spawn_regions_layer->PopSpawnRegion();
    });
    remove_team->CenterLabel();
    remove_team->SetAlignment(WidgetAlignment_BottomRight);

    GetWidgetManager()->AddWidget(add_team);
    GetWidgetManager()->AddWidget(remove_team);
}

TeamListWindow::~TeamListWindow() = default;
