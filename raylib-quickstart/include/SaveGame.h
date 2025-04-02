#ifndef SAVEGAME_H
#define SAVEGAME_H

#include <vector>
#include <string>

// Estructura para almacenar nombre y puntuaci�n
struct ScoreEntry {
    std::string name;
    int score;

    // Constructor para inicializar la entrada
    ScoreEntry(std::string n, int s) : name(n), score(s) {}
};

class SaveGame {
public:
    // M�todo para guardar el nombre y la puntuaci�n en el archivo
    static void SaveNameToFile(const std::string& name, int score);

    // M�todo para obtener todas las entradas (nombre, puntuaci�n)
    static std::vector<ScoreEntry> GetEntriesFromFile();

    // M�todo para guardar las entradas ordenadas
    static void SaveEntriesToFile(const std::vector<ScoreEntry>& entries);
};

#endif // SAVEGAME_H
