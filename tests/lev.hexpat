import type.color;
import type.magic;
import std.string;

#pragma pattern_limit 262144
#pragma endian little

u32 g_grid_width @ 0x03;
u32 g_grid_height @ 0x07;

////////////////////////////////
// Simple CString structure
// (copied from https://github.com/WerWolv/ImHex-Patterns/blob/21a0b99eaca5609e3adbe8bdf7adba56072b0d3e/includes/std/string.pat#L38C12-L38C36 )

struct MyNullStringBase<DataType> {
    DataType data[while(std::mem::read_unsigned($, sizeof(DataType)) != 0x00)];
    DataType null_terminator;
} [[sealed, format("std::string::impl::format_string"), transform("std::string::impl::format_string")]];
using CString = MyNullStringBase<char>;


///////////////////////////////
// Bitmap

// Use less memory
//struct Color {
//    u8 r, g, b, a;
//} [[color("444444")]];

struct Bitmap {
    type::Magic<"img"> sign_start;
    u32 image_width;
    u32 image_height;
    //Color impage_pixels[image_width*image_height];
    type::RGBA8 impage_pixels[image_width*image_height];
    type::Magic<"gmi"> sign_end;
};


///////////////////////////////
// Spawn region layer

struct SpawnRegion {
    float x;
    float y;
    float width;
    float height;
};

struct LayerSpawnRegion {
    type::Magic<"reg"> sign_start;
    u32 spawn_region_count;
    SpawnRegion spawn_regions[spawn_region_count];
    type::Magic<"ger"> sign_end;
};


///////////////////////////////
// Tile layer

struct LayerTilemap {
    type::Magic<"til"> sign_start;

    u32 tile_width;
    u32 tile_height;

    u32 tileset_lock;

    if(tileset_lock == 0) {
        Bitmap tileset_bitmap;
    }

    u8 tilemap[g_grid_width * g_grid_height];

    type::Magic<"lit"> sign_end;
};



///////////////////////////////
// Layer header

enum LayerType : u32 {
    None = 0,
    Tilemap = 1,
    SpawnRegions = 2
};


struct LayerHeader {
    type::Magic<"lay"> sign;
    CString layer_name;
    LayerType layer_type;

    if(layer_type == LayerType::Tilemap) {
        LayerTilemap layer_tilemap;
    }
    else if(layer_type == LayerType::SpawnRegions) {
        LayerSpawnRegion layer_spawn_region;
    }
};



struct EditorLevel {
    type::Magic<"lev"> sign_start;

    u32 grid_width;
    u32 grid_height;
    float terrain_width;
    float terrain_height;

    u32 layer_count;

    LayerHeader layers[layer_count];

    type::Magic<"vel"> sign_end;
};

EditorLevel g_level @ 0x0;
