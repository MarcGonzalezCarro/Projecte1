#include "SaveGame.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <algorithm>  // Para std::sort

#define FILENAME "resources/highscores.txt"

// Implementación de SaveNameToFile
void SaveGame::SaveNameToFile(const std::string& name, int score, int enemiesKilled, int ballomKills, int onilKills, int dahlKills, int minvoKills, int doriaKills, int ovapeKills, 
    int passKills, int pontanKills, int bombsPlanted, int softBlocksDestroyed, int powerUpsPicked, int fireUpCounter, int bombUpCounter, int speedUpCounter, 
    int flamePassCounter, int bombPassCounter, int InvincibleCounter, int remoteControlCounter, int wallPassCounter)
{
    std::ofstream file(FILENAME, std::ios::app); // Abrir archivo en modo "append"
    if (file.is_open()) {
        // Escribir todos los datos en el archivo, separados por comas
        file << name << ","
            << score << ","
            << enemiesKilled << ","
            << ballomKills << ","
            << onilKills << ","
            << dahlKills << ","
            << minvoKills << ","
            << doriaKills << ","
            << ovapeKills << ","
            << passKills << ","
            << pontanKills << ","
            << bombsPlanted << ","
            << softBlocksDestroyed << ","
            << powerUpsPicked << ","
            << fireUpCounter << ","
            << bombUpCounter << ","
            << speedUpCounter << ","
            << flamePassCounter << ","
            << bombPassCounter << ","
            << InvincibleCounter << ","
            << remoteControlCounter << ","
            << wallPassCounter
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
            int score, enemiesKilled, ballomKills, onilKills, dahlKills, minvoKills, doriaKills,
                ovapeKills, passKills, pontanKills, bombsPlanted, softBlocksDestroyed,
                powerUpsPicked, fireUpCounter, bombUpCounter, speedUpCounter, flamePassCounter,
                bombPassCounter, InvincibleCounter, remoteControlCounter, wallPassCounter;

            // Leer los datos del archivo, separados por comas
            std::getline(ss, name, ',');         // Nombre
            ss >> score;                         // Puntuación
            ss.ignore(1, ',');                   // Ignorar la coma después de la puntuación

            ss >> enemiesKilled;                 // Kills de enemigos
            ss.ignore(1, ','); 
            ss >> ballomKills;                   // Kills de Ballom
            ss.ignore(1, ',');
            ss >> onilKills;                     // Kills de Onil
            ss.ignore(1, ',');
            ss >> dahlKills;                     // Kills de Dahl
            ss.ignore(1, ',');
            ss >> minvoKills;                    // Kills de Minvo
            ss.ignore(1, ',');
            ss >> doriaKills;                    // Kills de Doria
            ss.ignore(1, ',');
            ss >> ovapeKills;                    // Kills de Ovape
            ss.ignore(1, ',');
            ss >> passKills;                     // Kills de Pass
            ss.ignore(1, ',');
            ss >> pontanKills;                   // Kills de Pontan
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
            ss.ignore(1, ',');
            ss >> flamePassCounter;              // Contador de Flame Pass
            ss.ignore(1, ',');
            ss >> bombPassCounter;               // Contador de Bomb Pass
            ss.ignore(1, ',');
            ss >> InvincibleCounter;             // Contador de Invincible
            ss.ignore(1, ',');
            ss >> remoteControlCounter;          // Contador de Remote Control
            ss.ignore(1, ',');
            ss >> wallPassCounter;               // Contador de Wall Pass

            // Crear un objeto ScoreEntry y agregarlo al vector
            entries.push_back(ScoreEntry(name, score, enemiesKilled, ballomKills, onilKills, dahlKills, minvoKills, doriaKills,
                ovapeKills, passKills, pontanKills, bombsPlanted,
                softBlocksDestroyed, powerUpsPicked, fireUpCounter,
                bombUpCounter, speedUpCounter, flamePassCounter, bombPassCounter, InvincibleCounter,
                remoteControlCounter, wallPassCounter));
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
                << entry.enemiesKilled << ","
                << entry.ballomKills << ","
                << entry.onilKills << ","
                << entry.dahlKills << ","
                << entry.minvoKills << ","
                << entry.doriaKills << ","
                << entry.ovapeKills << ","
                << entry.passKills << ","
                << entry.pontanKills << ","
                << entry.bombsPlanted << ","
                << entry.softBlocksDestroyed << ","
                << entry.powerUpsPicked << ","
                << entry.fireUpCounter << ","
                << entry.bombUpCounter << ","
                << entry.speedUpCounter << ","
                << entry.flamePassCounter << ","
                << entry.bombPassCounter << ","
                << entry.InvincibleCounter << ","
                << entry.remoteControlCounter << ","
                << entry.wallPassCounter
                << std::endl;  // Añadir salto de línea al final de cada entrada
        }
        file.close();
    }
    else {
        std::cerr << "Error al abrir el archivo para guardar." << std::endl;
    }
}

