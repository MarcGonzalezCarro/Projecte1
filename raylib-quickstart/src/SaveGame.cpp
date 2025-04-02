#include "SaveGame.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <algorithm>  // Para std::sort

#define FILENAME "resources/highscores.txt"

// Implementación de SaveNameToFile
void SaveGame::SaveNameToFile(const std::string& name, int score) {
    std::ofstream file(FILENAME, std::ios::app); // Abrir archivo en modo "append"
    if (file.is_open()) {
        file << name << "," << score << std::endl;  // Guardar el nombre y la puntuación
        file.close();
    }
    else {
        std::cerr << "Error al abrir el archivo para guardar." << std::endl;
    }
}

// Implementación de GetEntriesFromFile
std::vector<ScoreEntry> SaveGame::GetEntriesFromFile() {
    std::vector<ScoreEntry> entries;
    std::ifstream file(FILENAME);  // Abrir archivo en modo "lectura"

    if (file.is_open()) {
        std::string line;
        while (std::getline(file, line)) {
            std::stringstream ss(line);
            std::string name;
            int score;

            // Leer el nombre y la puntuación del archivo
            std::getline(ss, name, ',');  // Dividir por la coma
            ss >> score;

            entries.push_back(ScoreEntry(name, score));  // Añadir la entrada al vector
        }
        file.close();
    }
    else {
        std::cerr << "Error al abrir el archivo para leer." << std::endl;
    }
    return entries;
}

// Implementación de SaveEntriesToFile
void SaveGame::SaveEntriesToFile(const std::vector<ScoreEntry>& entries) {
    std::ofstream file(FILENAME, std::ios::trunc); // Abrir el archivo en modo "escritura" y truncar (borrar contenido)
    if (file.is_open()) {
        // Guardar las entradas ordenadas en el archivo
        for (const auto& entry : entries) {
            file << entry.name << "," << entry.score << std::endl;
        }
        file.close();
    }
    else {
        std::cerr << "Error al abrir el archivo para guardar." << std::endl;
    }
}
