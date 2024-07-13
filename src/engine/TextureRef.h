#ifndef TEXTUREREF_H
#define TEXTUREREF_H

#include <raylib.h>
#include <memory>
#include <string>


class TextureRef {
public:
    static TextureRef construct(std::string path);
    static TextureRef construct(Texture texture);       //NOTE : we shoud not create two sets of TextureRefs of the same texture.
    static TextureRef construct(Image image);

    TextureRef();           //used for invalid textures
    TextureRef(const TextureRef &other);          //copy constructor
    ~TextureRef();

    TextureRef& operator=(const TextureRef& other);

    inline Texture GetTexture() { return m_texture; }
    inline Texture *GetTexturePtr() { return &m_texture; }
    inline bool IsValid() { return (m_counter != nullptr && m_texture.id > 0); }

private:
    explicit TextureRef(Texture &texture);

    Texture m_texture;
    std::shared_ptr<int> m_counter;
};


#endif //TEXTUREREF_H
