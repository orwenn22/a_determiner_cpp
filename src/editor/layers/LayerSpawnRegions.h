#ifndef UI_FRAMEWORK_LAYERSPAWNREGIONS_H
#define UI_FRAMEWORK_LAYERSPAWNREGIONS_H

#include "Layer.h"

#include <vector>

#include "../EditorSpawnRegion.h"

class LayerSpawnRegions : public Layer {
public:
    explicit LayerSpawnRegions(EditorLevel *level);
    ~LayerSpawnRegions() override;

    void UpdateIfSelected(EditorState *editor) override;
    void Update(EditorState *editor) override;
    void PreDraw(EditorState *editor) override;
    void Draw(EditorState *editor) override;

    void AddSpawnRegion(EditorSpawnRegion region);
    EditorSpawnRegion *GetSpawnRegion(int index);
    int GetSpawnRegionCount();
    void PopSpawnRegion();

    bool Save(FILE *out_file) override;
    static LayerSpawnRegions *Load(EditorLevel *level, FILE *in_file);

private:
    std::vector<EditorSpawnRegion> m_spawn_regions;
    bool m_selected;
};


#endif //UI_FRAMEWORK_LAYERSPAWNREGIONS_H
