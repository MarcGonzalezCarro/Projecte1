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
    textures.push_back(LoadTexture("resources/BomberMan2.png")); //1
    textures.push_back(LoadTexture("resources/Blast01.png")); //2
    textures.push_back(LoadTexture("resources/Bomb.png")); //3
    
    textures.push_back(LoadTexture("resources/Ballom.png")); //4
    textures.push_back(LoadTexture("resources/Onil.png")); //5
    textures.push_back(LoadTexture("resources/Dahl.png")); //6
    textures.push_back(LoadTexture("resources/Minvo.png")); //7
    textures.push_back(LoadTexture("resources/Doria.png")); //8
    textures.push_back(LoadTexture("resources/Ovape.png")); //9
    textures.push_back(LoadTexture("resources/Pass.png")); //10
    textures.push_back(LoadTexture("resources/Pontan.png")); //11
    
    
    
    textures.push_back(LoadTexture("resources/Wall.png")); //12
    textures.push_back(LoadTexture("resources/PowerUps.png")); //13
    textures.push_back(LoadTexture("resources/Exit.png")); //14
    textures.push_back(LoadTexture("resources/Bomberman-TitleScreen.png")); //15
    textures.push_back(LoadTexture("resources/Flecha.png")); //16

    textures.push_back(LoadTexture("resources/MechaBlastX.png")); //17
}

void ResourceManager::LoadMusic() {
    musics.push_back(LoadMusicStream("resources/Title_Screen.mp3")); //0
    musics.push_back(LoadMusicStream("resources/Main_BGM.mp3")); //1
    musics.push_back(LoadMusicStream("resources/Power-Up_Get.mp3")); //2
    musics.push_back(LoadMusicStream("resources/Stage.mp3")); //3
    musics.push_back(LoadMusicStream("resources/Dead.mp3")); //4
}

void ResourceManager::LoadSounds() {
    sounds.push_back(LoadSound("resources/Bomberman SFX (1).wav"));
    sounds.push_back(LoadSound("resources/Bomberman SFX (2).wav"));
    sounds.push_back(LoadSound("resources/Bomberman SFX (3).wav"));
    sounds.push_back(LoadSound("resources/Bomberman SFX (4).wav"));
    sounds.push_back(LoadSound("resources/Bomberman SFX (5).wav"));
    sounds.push_back(LoadSound("resources/Bomberman SFX (6).wav"));
    sounds.push_back(LoadSound("resources/Bomb Sound.mp3"));
}

Music ResourceManager::GetMusic(size_t index) {
    if (index < musics.size()) {
        return musics[index];
    }
}

Sound ResourceManager::GetSound(size_t index)
{
    if (index < sounds.size()) {
        return sounds[index];
    }
}
