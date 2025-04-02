#ifndef SAVEGAME_H
#define SAVEGAME_H

#include <vector>
#include <string>

// Estructura para almacenar nombre y puntuación
struct ScoreEntry {
    std::string name;
    int score;

    // Constructor para inicializar la entrada
    ScoreEntry(std::string n, int s) : name(n), score(s) {}
};

class SaveGame {
public:
    // Método para guardar el nombre y la puntuación en el archivo
    static void SaveNameToFile(const std::string& name, int score);

    // Método para obtener todas las entradas (nombre, puntuación)
    static std::vector<ScoreEntry> GetEntriesFromFile();

    // Método para guardar las entradas ordenadas
    static void SaveEntriesToFile(const std::vector<ScoreEntry>& entries);
};

#endif // SAVEGAME_H
