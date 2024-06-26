#ifndef UI_FRAMEWORK_LAYERSPAWNREGIONS_H
#define UI_FRAMEWORK_LAYERSPAWNREGIONS_H

#include "Layer.h"

#include <vector>

#include "../EditorSpawnRegion.h"

class LayerSpawnRegions : public Layer {
public:
    explicit LayerSpawnRegions(EditorState *editor);
    ~LayerSpawnRegions() override;

    void UpdateIfSelected() override;
    void Update() override;
    void Draw() override;

    void AddSpawnRegion(EditorSpawnRegion region);

private:
    std::vector<EditorSpawnRegion> m_spawn_regions;
};


#endif //UI_FRAMEWORK_LAYERSPAWNREGIONS_H
