#ifndef SAVEGAME_H
#define SAVEGAME_H

#include <vector>
#include <string>

// Estructura para almacenar nombre y puntuación
struct ScoreEntry {
    std::string name;
    int score;
    int ballomKills;
    int onilKills;
    int dahlKills;
    int minvoKills;
    int doriaKills;
    int ovapeKills;
    int passKills;
    int pontanKills;
    int bombsPlanted;
    int softBlocksDestroyed;
    int powerUpsPicked;
    int fireUpCounter;
    int bombUpCounter;
    int speedUpCounter;
    // Constructor para inicializar la entrada
    ScoreEntry(std::string name, int score, int ballomKills = 0, int onilKills = 0, int dahlKills = 0, int minvoKills = 0, int doriaKills = 0, 
        int ovapeKills = 0, int passKills = 0, int pontanKills = 0, 
        int bombsPlanted = 0, int softBlocksDestroyed = 0, int powerUpsPicked = 0, int fireUpCounter = 0,
        int bombUpCounter = 0, int speedUpCounter = 0)
        : name(name), score(score), ballomKills(ballomKills), onilKills(onilKills), dahlKills(dahlKills), minvoKills(minvoKills),
        doriaKills(doriaKills), ovapeKills(ovapeKills), passKills(passKills), pontanKills(pontanKills),
        bombsPlanted(bombsPlanted), softBlocksDestroyed(softBlocksDestroyed),
        powerUpsPicked(powerUpsPicked), fireUpCounter(fireUpCounter),
        bombUpCounter(bombUpCounter), speedUpCounter(speedUpCounter) {}
};

class SaveGame {
public:
    // Método para guardar el nombre y la puntuación en el archivo
    static void SaveNameToFile(const std::string& name, int score, int ballomKills, int onilKills, int dahlKills, int minvoKills, int doriaKills, int ovapeKills, int passKills, int pontanKills, int bombsPlanted, int softBlocksDestroyed, int powerUpsPicked, int fireUpCounter, int bombUpCounter, int speedUpCounter);

    // Método para obtener todas las entradas (nombre, puntuación)
    static std::vector<ScoreEntry> GetEntriesFromFile();

    // Método para guardar las entradas ordenadas
    static void SaveEntriesToFile(const std::vector<ScoreEntry>& entries);
};

#endif // SAVEGAME_H
