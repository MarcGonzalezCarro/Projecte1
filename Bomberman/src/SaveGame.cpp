#include "SaveGame.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <algorithm>  // Para std::sort

#define FILENAME "resources/highscores.txt"

// Implementación de SaveNameToFile
void SaveGame::SaveNameToFile(const std::string& name, int score, int ballomKills, int doriaKills, int bombsPlanted, int softBlocksDestroyed, int powerUpsPicked, int fireUpCounter, int bombUpCounter, int speedUpCounter)
{
    std::ofstream file(FILENAME, std::ios::app); // Abrir archivo en modo "append"
    if (file.is_open()) {
        // Escribir todos los datos en el archivo, separados por comas
        file << name << ","
            << score << ","
            << ballomKills << ","
            << doriaKills << ","
            << bombsPlanted << ","
            << softBlocksDestroyed << ","
            << powerUpsPicked << ","
            << fireUpCounter << ","
            << bombUpCounter << ","
            << speedUpCounter
            << std::endl;  // Añadir salto de línea al final de cada entrada
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
            int score, ballomKills, doriaKills, bombsPlanted, softBlocksDestroyed,
                powerUpsPicked, fireUpCounter, bombUpCounter, speedUpCounter;

            // Leer los datos del archivo, separados por comas
            std::getline(ss, name, ',');         // Nombre
            ss >> score;                         // Puntuación
            ss.ignore(1, ',');                   // Ignorar la coma después de la puntuación

            ss >> ballomKills;                   // Kills de Ballom
            ss.ignore(1, ',');
            ss >> doriaKills;                    // Kills de Doria
            ss.ignore(1, ',');
            ss >> bombsPlanted;                  // Bombas plantadas
            ss.ignore(1, ',');
            ss >> softBlocksDestroyed;           // Bloques destruidos
            ss.ignore(1, ',');
            ss >> powerUpsPicked;                // Power-ups recogidos
            ss.ignore(1, ',');
            ss >> fireUpCounter;                 // Contador de Fire Up
            ss.ignore(1, ',');
            ss >> bombUpCounter;                 // Contador de Bomb Up
            ss.ignore(1, ',');
            ss >> speedUpCounter;                // Contador de Speed Up

            // Crear un objeto ScoreEntry y agregarlo al vector
            entries.push_back(ScoreEntry(name, score, ballomKills, doriaKills, bombsPlanted,
                softBlocksDestroyed, powerUpsPicked, fireUpCounter,
                bombUpCounter, speedUpCounter));
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
            file << entry.name << ","
                << entry.score << ","
                << entry.ballomKills << ","
                << entry.doriaKills << ","
                << entry.bombsPlanted << ","
                << entry.softBlocksDestroyed << ","
                << entry.powerUpsPicked << ","
                << entry.fireUpCounter << ","
                << entry.bombUpCounter << ","
                << entry.speedUpCounter
                << std::endl;  // Añadir salto de línea al final de cada entrada
        }
        file.close();
    }
    else {
        std::cerr << "Error al abrir el archivo para guardar." << std::endl;
    }
}

