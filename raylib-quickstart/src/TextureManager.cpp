#include "TextureManager.h"
#include <stdexcept> // Para std::runtime_error

// Constructor: Carga todas las texturas
TextureManager::TextureManager() {

    
    
    // A�ade m�s texturas seg�n sea necesario
}

// Obtener una textura por �ndice
Texture2D TextureManager::GetTexture(size_t index) const {
    if (index < textures.size()) {
        return textures[index];
    }
    else {
        throw std::runtime_error("�ndice de textura fuera de rango");
    }
}

// Destructor: Liberar todas las texturas
TextureManager::~TextureManager() {
    for (Texture2D texture : textures) {
        UnloadTexture(texture);
    }
}

void TextureManager::LoadTextures() {
    textures.push_back(LoadTexture("resources/BomberMan.png"));
    textures.push_back(LoadTexture("resources/Blast01.png"));
    textures.push_back(LoadTexture("resources/bomb01.png"));
    textures.push_back(LoadTexture("resources/Doria01.png"));
    textures.push_back(LoadTexture("resources/test.png"));
}
