#include "ResourceManager.h"
#include <stdexcept> // Para std::runtime_error

// Constructor: Carga todas las texturas
ResourceManager::ResourceManager() {

    
    
    // Añade más texturas según sea necesario
}

// Obtener una textura por índice
Texture2D ResourceManager::GetTexture(size_t index) const {
    if (index < textures.size()) {
        return textures[index];
    }
    else {
        throw std::runtime_error("Índice de textura fuera de rango");
    }
}

// Destructor: Liberar todas las texturas
ResourceManager::~ResourceManager() {
    for (Texture2D texture : textures) {
        UnloadTexture(texture);
    }
}

void ResourceManager::LoadTextures() {
    textures.push_back(LoadTexture("resources/BomberMan.png")); //0
    textures.push_back(LoadTexture("resources/Blast01.png")); //1
    textures.push_back(LoadTexture("resources/Bomb.png")); //2
    textures.push_back(LoadTexture("resources/Doria.png")); //
    
    textures.push_back(LoadTexture("resources/Ballom.png")); //
    
    textures.push_back(LoadTexture("resources/Wall.png")); //5
    textures.push_back(LoadTexture("resources/PowerUps.png")); //6
    textures.push_back(LoadTexture("resources/Exit.png")); //7
    textures.push_back(LoadTexture("resources/Bomberman-TitleScreen.png")); //8
    textures.push_back(LoadTexture("resources/Flecha.png")); //8
}

void ResourceManager::LoadMusic() {
    musics.push_back(LoadMusicStream("resources/Title_Screen.mp3")); //0
    musics.push_back(LoadMusicStream("resources/Main_BGM.mp3")); //1
    musics.push_back(LoadMusicStream("resources/Power-Up_Get.mp3")); //2
}

Music ResourceManager::GetMusic(size_t index) {
    if (index < musics.size()) {
        return musics[index];
    }
}