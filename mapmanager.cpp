#include "MapManager.h"
#include <fstream>
#include <iostream>
#include <algorithm>

MapManager::MapManager(int width, int height) : m_width(width), m_height(height) {
    m_width = std::max(MIN_GRID_WIDTH, std::min(width, MAX_GRID_WIDTH));
    m_height = std::max(MIN_GRID_HEIGHT, std::min(height, MAX_GRID_HEIGHT));
    m_grid.resize(static_cast<size_t>(m_height), std::vector<int>(static_cast<size_t>(m_width), 0));
    clear();
}

MapManager::~MapManager() = default;

void MapManager::resize(int width, int height) {
    m_width = std::max(MIN_GRID_WIDTH, std::min(width, MAX_GRID_WIDTH));
    m_height = std::max(MIN_GRID_HEIGHT, std::min(height, MAX_GRID_HEIGHT));
    m_grid.clear();
    m_grid.resize(static_cast<size_t>(m_height), std::vector<int>(static_cast<size_t>(m_width), 0));
    clear();
}

void MapManager::clear() {
    for (int i = 0; i < m_height; i++) {
        for (int j = 0; j < m_width; j++) {
            m_grid[static_cast<size_t>(i)][static_cast<size_t>(j)] = 0;
        }
    }
}

void MapManager::setCell(int x, int y, int value) {
    if (x >= 0 && x < m_width && y >= 0 && y < m_height) {
        if (value >= 0 && value <= 7) {
            m_grid[static_cast<size_t>(y)][static_cast<size_t>(x)] = value;
        }
    }
}

int MapManager::getCell(int x, int y) const {
    if (x >= 0 && x < m_width && y >= 0 && y < m_height) {
        return m_grid[static_cast<size_t>(y)][static_cast<size_t>(x)];
    }
    return 0;
}

void MapManager::loadPreset(int /*presetId*/) {
    clear();
}

void MapManager::loadFromFile(const std::string& filename) {
    std::ifstream file(filename);
    if (file.is_open()) {
        int width = 0, height = 0;
        file >> width >> height;
        if (width > 0 && height > 0 && width <= MAX_GRID_WIDTH && height <= MAX_GRID_HEIGHT) {
            resize(width, height);
            for (int i = 0; i < m_height && i < height; i++) {
                for (int j = 0; j < m_width && j < width; j++) {
                    int value = 0;
                    file >> value;
                    if (value >= 0 && value <= 7) {
                        m_grid[static_cast<size_t>(i)][static_cast<size_t>(j)] = value;
                    }
                }
            }
        }
        file.close();
    }
}

void MapManager::saveToFile(const std::string& filename) {
    std::ofstream file(filename);
    if (file.is_open()) {
        file << m_width << " " << m_height << '\n';
        for (int i = 0; i < m_height; i++) {
            for (int j = 0; j < m_width; j++) {
                file << m_grid[static_cast<size_t>(i)][static_cast<size_t>(j)] << " ";
            }
            file << '\n';
        }
        file.close();
    }
}

void MapManager::createLevel(int /*level*/) {
    clear();
}