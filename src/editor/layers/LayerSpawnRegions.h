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
    void PreDraw() override;
    void Draw() override;
    void Save(FILE *out_file) override;

    void AddSpawnRegion(EditorSpawnRegion region);

    static LayerSpawnRegions *Load(EditorState *editor, FILE *in_file);

private:
    std::vector<EditorSpawnRegion> m_spawn_regions;
    bool m_selected;
};


#endif //UI_FRAMEWORK_LAYERSPAWNREGIONS_H
