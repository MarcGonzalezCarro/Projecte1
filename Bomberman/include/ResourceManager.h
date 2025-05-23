#ifndef RESOURCEMANAGER_H
#define RESOURCEMANAGER_H

#include "raylib.h"
#include <vector>
#include <cstddef> // Para size_t

class ResourceManager {
private:
    std::vector<Texture2D> textures;
    std::vector<Music> musics;
    std::vector<Sound> sounds;
public:
    ResourceManager(); // Constructor que carga todas las texturas
    Texture2D GetTexture(size_t index) const; // Obtiene una textura por �ndice
    void LoadTextures();
    void LoadMusic();
    void LoadSounds();
    Music GetMusic(size_t index);
    Sound GetSound(size_t index);
    ~ResourceManager(); // Destructor que libera las texturas
};

#endif 