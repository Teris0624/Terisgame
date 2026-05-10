#include "SettingsManager.h"
#include <fstream>
#include <iostream>

SettingsManager::SettingsManager() {
    resetToDefaults();
}

SettingsManager::~SettingsManager() {
    saveToFile("settings.cfg");
}

GameSettings SettingsManager::getSettings() const {
    return m_settings;
}

void SettingsManager::setSettings(const GameSettings& settings) {
    m_settings = settings;
}

void SettingsManager::saveToFile(const std::string& filename) {
    std::ofstream file(filename);
    if (file.is_open()) {
        file << "grid_width=" << m_settings.gridWidth << '\n';
        file << "grid_height=" << m_settings.gridHeight << '\n';
        file << "initial_fall_speed=" << m_settings.initialFallSpeed << '\n';
        file << "score_multiplier=" << m_settings.scoreMultiplier << '\n';
        file << "sound_enabled=" << m_settings.soundEnabled << '\n';
        file << "show_ghost_piece=" << m_settings.showGhostPiece << '\n';
        file.close();
    }
}

void SettingsManager::loadFromFile(const std::string& filename) {
    std::ifstream file(filename);
    if (file.is_open()) {
        std::string line;
        while (std::getline(file, line)) {
            size_t pos = line.find('=');
            if (pos != std::string::npos) {
                std::string key = line.substr(0, pos);
                std::string value = line.substr(pos + 1);
                try {
                    if (key == "grid_width") m_settings.gridWidth = std::stoi(value);
                    else if (key == "grid_height") m_settings.gridHeight = std::stoi(value);
                    else if (key == "initial_fall_speed") m_settings.initialFallSpeed = std::stof(value);
                    else if (key == "score_multiplier") m_settings.scoreMultiplier = std::stoi(value);
                    else if (key == "sound_enabled") m_settings.soundEnabled = (value == "1");
                    else if (key == "show_ghost_piece") m_settings.showGhostPiece = (value == "1");
                }
                catch (...) {}
            }
        }
        file.close();
    }
}

void SettingsManager::resetToDefaults() {
    m_settings.gridWidth = DEFAULT_GRID_WIDTH;
    m_settings.gridHeight = DEFAULT_GRID_HEIGHT;
    m_settings.initialFallSpeed = 0.5f;
    m_settings.scoreMultiplier = 1;
    m_settings.soundEnabled = true;
    m_settings.showGhostPiece = true;
}