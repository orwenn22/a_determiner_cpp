#ifndef UI_FRAMEWORK_LAYER_H
#define UI_FRAMEWORK_LAYER_H

#include <string>

enum LayerType {
    LayerType_None,
    LayerType_Tilemap,
    LayerType_SpawnRegions
};

class EditorState;

class Layer {
public:
    Layer(EditorState *editor, std::string name, LayerType layer_type = LayerType_None);
    virtual ~Layer();

    virtual void Update();
    virtual void UpdateIfSelected();
    virtual void Draw();
    virtual void HandleFileDrag(std::string file_name);

    inline EditorState *Editor() { return m_editor; }
    inline LayerType Type() { return m_layer_type; }
    inline std::string Name() { return m_name; }

private:
    EditorState *m_editor;
    LayerType m_layer_type;
    std::string m_name;
};


#endif //UI_FRAMEWORK_LAYER_H
