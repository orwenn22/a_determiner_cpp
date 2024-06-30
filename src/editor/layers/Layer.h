#ifndef UI_FRAMEWORK_LAYER_H
#define UI_FRAMEWORK_LAYER_H

#include <string>

enum LayerType {
    LayerType_None,
    LayerType_Tilemap,
    LayerType_SpawnRegions
};

class EditorLevel;
class EditorState;

class Layer {
public:
    Layer(EditorLevel *level, std::string name, LayerType layer_type = LayerType_None);
    virtual ~Layer();

    //Methods for the editor
    virtual void Update(EditorState *editor);
    virtual void UpdateIfSelected(EditorState *editor);
    virtual void PreDraw(EditorState *editor);
    virtual void Draw(EditorState *editor);
    virtual void HandleFileDrag(EditorState *editor, std::string file_name);

    virtual void Save(FILE *out_file);
    static Layer *Load(EditorLevel *editor, FILE *in_file);

    inline EditorLevel *Level() { return m_level; }
    inline LayerType Type() { return m_layer_type; }
    inline std::string Name() { return m_name; }

private:
    EditorLevel *m_level;
    LayerType m_layer_type;
    std::string m_name;
};


#endif //UI_FRAMEWORK_LAYER_H
