#ifndef A_DETERMINER_LAYER_H
#define A_DETERMINER_LAYER_H

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

    virtual bool Savable();
    virtual bool Save(FILE *out_file);
    static Layer *Load(EditorLevel *level, FILE *in_file);

    inline EditorLevel *Level() { return m_level; }
    inline LayerType Type() { return m_layer_type; }
    inline std::string Name() { return m_name; }

private:
    EditorLevel *m_level;
    LayerType m_layer_type;
    std::string m_name;
};


#endif //A_DETERMINER_LAYER_H
