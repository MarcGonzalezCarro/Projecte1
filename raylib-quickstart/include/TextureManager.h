#ifndef TEXTUREMANAGER_H
#define TEXTUREMANAGER_H

#include "raylib.h"
#include <vector>
#include <cstddef> // Para size_t

class TextureManager {
private:
    std::vector<Texture2D> textures;

public:
    TextureManager(); // Constructor que carga todas las texturas
    Texture2D GetTexture(size_t index) const; // Obtiene una textura por índice
    void LoadTextures();
    ~TextureManager(); // Destructor que libera las texturas
};

#endif // TEXTUREMANAGER_H