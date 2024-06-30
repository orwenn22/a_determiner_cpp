#include "Layer.h"

#include "engine/util/Trace.h"
#include "utils/FileOp.h"
#include "LayerSpawnRegions.h"
#include "LayerTilemap.h"


Layer::Layer(EditorLevel *level, std::string name, LayerType layer_type) {
    m_level = level;
    m_name = name;
    m_layer_type = layer_type;
}

Layer::~Layer() = default;
void Layer::Update(EditorState *editor) { }
void Layer::UpdateIfSelected(EditorState *editor) { }
void Layer::PreDraw(EditorState *editor) { }
void Layer::Draw(EditorState *editor) { }
void Layer::HandleFileDrag(EditorState *editor, std::string file_name) { }

void Layer::Save(FILE *out_file) {
    fputs("lay", out_file);             //signature
    fputs(m_name.c_str(), out_file);    //layer name
    fputc(0, out_file);                 //layer name (end of string)
    WriteU32(m_layer_type, out_file);      //layer type;
}


Layer *Layer::Load(EditorLevel *level, FILE *in_file) {
    if(in_file == nullptr) return nullptr;

    if(!CheckSignature("lay", 3, in_file)) {
        TRACE("No 'lay' signature\n");
        return nullptr;
    }

    char name_buf[256] = { 0 };
    int i;
    for(i = 0; i < 256; ++i) {
        int c_int = getc(in_file);
        if(c_int == EOF) return nullptr;    //TODO : error message
        name_buf[i] = (char) c_int;
        if(c_int == 0) break;
    }
    if(i == 256) return nullptr;        //TODO : error message

    LayerType layer_type = (LayerType) ReadU32(in_file);
    Layer *r = nullptr;

    TRACE("Parsing layer '%s' of type %i\n", name_buf, layer_type);

    switch (layer_type) {
        case LayerType_SpawnRegions:
            r = LayerSpawnRegions::Load(level, in_file);
            break;
        case LayerType_Tilemap:
            r = LayerTilemap::Load(level, in_file);
            break;
        default:
            break;
    }

    if(r != nullptr) {
        r->m_name = std::string(name_buf);
        TRACE("Successfully parsed layer '%s' of type %i\n", name_buf, layer_type);
    }

    return r;
}
