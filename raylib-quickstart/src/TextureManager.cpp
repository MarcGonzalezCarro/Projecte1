#include "TextureManager.h"
#include <stdexcept> // Para std::runtime_error

// Constructor: Carga todas las texturas
TextureManager::TextureManager() {

    
    
    // Añade más texturas según sea necesario
}

// Obtener una textura por índice
Texture2D TextureManager::GetTexture(size_t index) const {
    if (index < textures.size()) {
        return textures[index];
    }
    else {
        throw std::runtime_error("Índice de textura fuera de rango");
    }
}

// Destructor: Liberar todas las texturas
TextureManager::~TextureManager() {
    for (Texture2D texture : textures) {
        UnloadTexture(texture);
    }
}

void TextureManager::LoadTextures() {
    textures.push_back(LoadTexture("resources/BomberMan.png")); //0
    textures.push_back(LoadTexture("resources/Blast01.png")); //1
    textures.push_back(LoadTexture("resources/Bomb.png")); //2
    textures.push_back(LoadTexture("resources/Doria.png")); //3
    textures.push_back(LoadTexture("resources/Ballom.png")); //4
    textures.push_back(LoadTexture("resources/Wall.png")); //5
    textures.push_back(LoadTexture("resources/PowerUps.png")); //6
    textures.push_back(LoadTexture("resources/Exit.png")); //7
}
