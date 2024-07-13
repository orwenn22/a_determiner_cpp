#include "TextureRef.h"

#include "util/Trace.h"

TextureRef TextureRef::construct(std::string path) {
    Texture texture = LoadTexture(path.c_str());
    if(texture.id <= 0) return TextureRef();
    return TextureRef(texture);
}

TextureRef TextureRef::construct(Texture texture) {
    if(texture.id <= 0) return TextureRef();
    return TextureRef(texture);
}

TextureRef TextureRef::construct(Image image) {
    Texture texture = LoadTextureFromImage(image);
    if(texture.id <= 0) return TextureRef();
    return TextureRef(texture);
}

TextureRef::TextureRef() : m_texture({0}), m_counter(nullptr) { }

TextureRef::TextureRef(Texture &texture) : m_texture(texture) {
    m_counter = std::make_shared<int>(1);
}

TextureRef::TextureRef(const TextureRef &other) : m_texture(other.m_texture), m_counter(other.m_counter) {
    if(m_counter == nullptr) return;
    ++(*m_counter);
    TRACE("Copied reference of texture %i, counter is now %i.\n", m_texture.id, (*m_counter));
}

TextureRef::~TextureRef() {
    if(m_counter == nullptr) return;
    --(*m_counter);
    if(*m_counter == 0) {
        TRACE("No more reference of texture %i, unloading...\n", m_texture.id);
        UnloadTexture(m_texture);
    }
    else {
        TRACE("Destroyed a reference of texture %i, counter is now %i.\n", m_texture.id, (*m_counter));
    }
}

TextureRef& TextureRef::operator=(const TextureRef& other) {
    if (this != &other) { // Protect against self-assignment
        //Handle "destruction" of the previous object
        if (m_counter && --(*m_counter) == 0) {
            TRACE("No more reference of texture %i, unloading...\n", m_texture.id);
            UnloadTexture(m_texture);
        }
        else {
            if(m_counter) { TRACE("Destroyed a reference of texture %i, counter is now %i.\n", m_texture.id, (*m_counter)); }
        }

        //Replace with other object
        m_texture = other.m_texture;
        m_counter = other.m_counter;
        if (m_counter) {
            ++(*m_counter);
            TRACE("Copied reference of texture %i, counter is now %i.\n", m_texture.id, (*m_counter));
        }
    }
    return *this;
}
