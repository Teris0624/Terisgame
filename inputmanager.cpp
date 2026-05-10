#include "InputManager.h"
#include <fstream>
#include <iostream>

InputManager::InputManager() {
    loadKeyBindings(getDefaultKeyBindings());
}

InputManager::~InputManager() {
    saveToFile("keys.cfg");
}

void InputManager::loadKeyBindings(const std::map<std::string, sf::Keyboard::Key>& bindings) {
    m_keyBindings = bindings;
}

std::map<std::string, sf::Keyboard::Key> InputManager::getKeyBindings() const {
    return m_keyBindings;
}

void InputManager::setKeyBinding(const std::string& action, sf::Keyboard::Key key) {
    m_keyBindings[action] = key;
}

sf::Keyboard::Key InputManager::getKeyBinding(const std::string& action) const {
    auto it = m_keyBindings.find(action);
    if (it != m_keyBindings.end()) {
        return it->second;
    }
    return sf::Keyboard::Key::Unknown;
}

bool InputManager::isActionPressed(const std::string& action) const {
    auto it = m_keyBindings.find(action);
    if (it != m_keyBindings.end()) {
        return sf::Keyboard::isKeyPressed(it->second);
    }
    return false;
}

bool InputManager::isActionJustPressed(const std::string& action) {
    auto it = m_keyBindings.find(action);
    if (it != m_keyBindings.end()) {
        sf::Keyboard::Key key = it->second;
        bool current = sf::Keyboard::isKeyPressed(key);
        bool previous = m_previousKeyStates[key];
        m_previousKeyStates[key] = current;
        return current && !previous;
    }
    return false;
}

void InputManager::update() {
    for (const auto& binding : m_keyBindings) {
        if (m_previousKeyStates.find(binding.second) == m_previousKeyStates.end()) {
            m_previousKeyStates[binding.second] = false;
        }
    }
}

void InputManager::saveToFile(const std::string& filename) {
    std::ofstream file(filename);
    if (file.is_open()) {
        for (const auto& binding : m_keyBindings) {
            file << binding.first << "=" << static_cast<int>(binding.second) << '\n';
        }
        file.close();
    }
}

void InputManager::loadFromFile(const std::string& filename) {
    std::ifstream file(filename);
    if (file.is_open()) {
        std::string line;
        while (std::getline(file, line)) {
            size_t pos = line.find('=');
            if (pos != std::string::npos) {
                std::string action = line.substr(0, pos);
                int keyCode = std::stoi(line.substr(pos + 1));
                m_keyBindings[action] = static_cast<sf::Keyboard::Key>(keyCode);
            }
        }
        file.close();
    }
}