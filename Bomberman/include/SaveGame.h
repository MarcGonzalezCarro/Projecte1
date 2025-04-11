#ifndef SAVEGAME_H
#define SAVEGAME_H

#include <vector>
#include <string>

// Estructura para almacenar nombre y puntuación
struct ScoreEntry {
    std::string name;
    int score;
    int ballomKills;
    int doriaKills;
    int bombsPlanted;
    int softBlocksDestroyed;
    int powerUpsPicked;
    int fireUpCounter;
    int bombUpCounter;
    int speedUpCounter;
    // Constructor para inicializar la entrada
    ScoreEntry(std::string name, int score, int ballomKills = 0, int doriaKills = 0, int bombsPlanted = 0,
        int softBlocksDestroyed = 0, int powerUpsPicked = 0, int fireUpCounter = 0,
        int bombUpCounter = 0, int speedUpCounter = 0)
        : name(name), score(score), ballomKills(ballomKills), doriaKills(doriaKills),
        bombsPlanted(bombsPlanted), softBlocksDestroyed(softBlocksDestroyed),
        powerUpsPicked(powerUpsPicked), fireUpCounter(fireUpCounter),
        bombUpCounter(bombUpCounter), speedUpCounter(speedUpCounter) {}
};

class SaveGame {
public:
    // Método para guardar el nombre y la puntuación en el archivo
    static void SaveNameToFile(const std::string& name, int score, int ballomKills, int doriaKills, int bombsPlanted, int softBlocksDestroyed, int powerUpsPicked, int fireUpCounter, int bombUpCounter, int speedUpCounter);

    // Método para obtener todas las entradas (nombre, puntuación)
    static std::vector<ScoreEntry> GetEntriesFromFile();

    // Método para guardar las entradas ordenadas
    static void SaveEntriesToFile(const std::vector<ScoreEntry>& entries);
};

#endif // SAVEGAME_H
