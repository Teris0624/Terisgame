#pragma once
#ifndef MAP_MANAGER_H
#define MAP_MANAGER_H

#include <vector>
#include <string>
#include "Constants.h"

class MapManager {
public:
    MapManager(int width, int height);
    ~MapManager();

    void resize(int width, int height);
    void clear();
    void setCell(int x, int y, int value);
    [[nodiscard]] int getCell(int x, int y) const;
    [[nodiscard]] int getWidth() const { return m_width; }
    [[nodiscard]] int getHeight() const { return m_height; }

    void loadPreset(int presetId);
    void loadFromFile(const std::string& filename);
    void saveToFile(const std::string& filename);
    void createLevel(int level);

private:
    int m_width;
    int m_height;
    std::vector<std::vector<int>> m_grid;
};

#endif