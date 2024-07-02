#ifndef A_DETERMINER_TEAMLISTWINDOW_H
#define A_DETERMINER_TEAMLISTWINDOW_H

#include "engine/windows/Window.h"


class LayerSpawnRegions;
class EditorState;

class TeamListWindow : public Window {
public:
    TeamListWindow(EditorState *editor, LayerSpawnRegions *spawn_regions_layer, int x, int y);
    ~TeamListWindow() override;

private:
    EditorState *m_editor;
    LayerSpawnRegions *m_spawn_regions_layer;
};



#endif //A_DETERMINER_TEAMLISTWINDOW_H
