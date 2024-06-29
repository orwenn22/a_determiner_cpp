#include "Layer.h"

Layer::Layer(EditorState *editor, std::string name, LayerType layer_type) {
    m_editor = editor;
    m_name = name;
    m_layer_type = layer_type;
}

Layer::~Layer() = default;
void Layer::Update() { }
void Layer::UpdateIfSelected() { }
void Layer::PreDraw() { }
void Layer::Draw() { }
void Layer::HandleFileDrag(std::string file_name) { }
void Layer::Save(FILE *out_file) { }
