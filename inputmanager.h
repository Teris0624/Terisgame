#pragma once
#ifndef INPUT_MANAGER_H
#define INPUT_MANAGER_H

#include <SFML/Window.hpp>
#include <map>
#include <string>
#include "Constants.h"

class InputManager {
public:
    InputManager();
    ~InputManager();

    void loadKeyBindings(const std::map<std::string, sf::Keyboard::Key>& bindings);
    std::map<std::string, sf::Keyboard::Key> getKeyBindings() const;
    void setKeyBinding(const std::string& action, sf::Keyboard::Key key);
    sf::Keyboard::Key getKeyBinding(const std::string& action) const;
    [[nodiscard]] bool isActionPressed(const std::string& action) const;
    bool isActionJustPressed(const std::string& action);
    void update();

    void saveToFile(const std::string& filename);
    void loadFromFile(const std::string& filename);

private:
    std::map<std::string, sf::Keyboard::Key> m_keyBindings;
    std::map<sf::Keyboard::Key, bool> m_previousKeyStates;
};

#endif
