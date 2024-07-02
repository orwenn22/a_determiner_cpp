#ifndef TEAMLISTWIDGET_H
#define TEAMLISTWIDGET_H

#include "engine/widgets/Widget.h"

class EditorState;
class LayerSpawnRegions;

class TeamListWidget : public Widget {
public:
    TeamListWidget(EditorState *editor, LayerSpawnRegions *spawn_regions_layer, int x, int y, int w, int h);
    ~TeamListWidget() override;

    void Update() override;
    void Draw() override;

private:
    EditorState *m_editor;
    LayerSpawnRegions *m_spawn_regions_layer;
};


#endif //TEAMLISTWIDGET_H
