#include "TetrisGame.h"
#include "Constants.h"

void TetrisGame::handleEvents() {
    if (!m_inputManager) return;
    m_inputManager->update();

    // 检查窗口大小是否改变
    static sf::Vector2u lastSize = m_window.getSize();
    sf::Vector2u currentSize = m_window.getSize();
    if (currentSize != lastSize) {
        lastSize = currentSize;
        if (m_uiManager) {
            m_uiManager->updateScale();
        }
        // SFML 3.1: 使用聚合初始化或直接传参创建 FloatRect
        // 方法1: 使用 sf::FloatRect{left, top, width, height}
        sf::FloatRect viewRect({ 0.0f, 0.0f },
            { static_cast<float>(currentSize.x),
            static_cast<float>(currentSize.y) });
        m_window.setView(sf::View(viewRect));
    }

    while (auto event = m_window.pollEvent()) {
        // ========== 窗口关闭事件 ==========
        if (event->is<sf::Event::Closed>()) {
            m_window.close();
            return;
        }

        // ========== 鼠标按键按下 ==========
        if (auto me = event->getIf<sf::Event::MouseButtonPressed>()) {
            if (me->button == sf::Mouse::Button::Left) {
                handleMouseClick(sf::Vector2i(me->position.x, me->position.y));
            }
        }

        // ========== 鼠标移动 ==========
        if (auto me = event->getIf<sf::Event::MouseMoved>()) {
            if (m_uiManager) {
                m_uiManager->handleMouseMove(sf::Vector2i(me->position.x, me->position.y));
            }
        }

        // ========== 键盘按键按下 ==========
        if (auto ke = event->getIf<sf::Event::KeyPressed>()) {
            // 按键设置等待状态（用于自定义按键）
            if (m_waitingForKey && m_state == GameState::KEY_SETTINGS) {
                if (ke->code != sf::Keyboard::Key::Escape) {
                    m_inputManager->setKeyBinding(m_waitingAction, ke->code);
                }
                m_waitingForKey = false;
                m_waitingAction.clear();
                continue;
            }

            // 根据游戏状态分发输入
            switch (m_state) {
            case GameState::MENU:
                handleMenuInput();
                break;
            case GameState::PLAYING:
                handlePlayingInput(ke->code);
                break;
            case GameState::PAUSED:
                handlePausedInput(ke->code);
                break;
            case GameState::SETTINGS:
                handleSettingsInput();
                break;
            case GameState::MAP_SETTINGS:
                handleMapSettingsInput();
                break;
            case GameState::KEY_SETTINGS:
                handleKeySettingsInput();
                break;
            case GameState::GAME_OVER:
                if (ke->code == sf::Keyboard::Key::Space) {
                    resetGame();
                }
                break;
            default:
                break;
            }
        }

        // ========== 键盘按键释放（用于连续移动）==========
        if (auto ke = event->getIf<sf::Event::KeyReleased>()) {
            if (m_state == GameState::PLAYING) {
                if (ke->code == m_inputManager->getKeyBinding("Move Left")) {
                    m_leftPressed = false;
                    m_moveDelay = 0.0f;
                }
                if (ke->code == m_inputManager->getKeyBinding("Move Right")) {
                    m_rightPressed = false;
                    m_moveDelay = 0.0f;
                }
            }
        }
    }
}

void TetrisGame::handleMouseClick(const sf::Vector2i& pos) {
    if (!m_uiManager) return;

    switch (m_state) {
        // ========== 主菜单鼠标点击 ==========
    case GameState::MENU: {
        int item = m_uiManager->getHoveredMenuItem();
        if (item >= 0) {
            m_selectedMenuItem = item;
            switch (item) {
            case 0: resetGame(); break;
            case 1: m_state = GameState::SETTINGS; m_selectedSettingsItem = 0; break;
            case 2: m_state = GameState::MAP_SETTINGS; m_selectedMapItem = 0; break;
            case 3: m_state = GameState::KEY_SETTINGS; m_selectedKeyItem = 0; break;
            case 4: m_window.close(); break;
            default: break;
            }
        }
        break;
    }

                        // ========== 暂停菜单鼠标点击 ==========
    case GameState::PAUSED: {
        int item = m_uiManager->getHoveredPauseItem();
        if (item >= 0) {
            m_selectedPauseItem = item;
            m_uiManager->setSelectedPauseItem(item);
            switch (item) {
            case 0: m_state = GameState::PLAYING; break;
            case 1:
                m_state = GameState::MENU;
                m_leftPressed = false;
                m_rightPressed = false;
                break;
            case 2: m_window.close(); break;
            default: break;
            }
        }
        break;
    }

                          // ========== 游戏结束菜单鼠标点击 ==========
    case GameState::GAME_OVER: {
        int item = m_uiManager->getHoveredGameOverItem();
        if (item >= 0) {
            switch (item) {
            case 0: resetGame(); break;
            case 1: m_state = GameState::MENU; break;
            case 2: m_window.close(); break;
            default: break;
            }
        }
        break;
    }

                             // ========== 设置菜单返回按钮鼠标点击 ==========
    case GameState::SETTINGS:
    case GameState::MAP_SETTINGS:
    case GameState::KEY_SETTINGS:
        if (m_uiManager->isBackButtonHovered()) {
            m_state = GameState::MENU;
        }
        break;

    default:
        break;
    }
}

void TetrisGame::handlePlayingInput(sf::Keyboard::Key key) {
    if (!m_inputManager) return;

    // 左右移动（按下立即响应）
    if (key == m_inputManager->getKeyBinding("Move Left")) {
        m_leftPressed = true;
        m_moveDelay = m_moveRepeatDelay;
        movePiece(-1, 0);
    }
    else if (key == m_inputManager->getKeyBinding("Move Right")) {
        m_rightPressed = true;
        m_moveDelay = m_moveRepeatDelay;
        movePiece(1, 0);
    }
    // 向下移动
    else if (m_inputManager->isActionJustPressed("Move Down")) {
        movePiece(0, 1);
    }
    // 旋转 - 立即响应，无延迟
    else if (m_inputManager->isActionJustPressed("Rotate")) {
        rotatePiece();  // 直接调用，无冷却
    }
    // 一键下落
    else if (m_inputManager->isActionJustPressed("Hard Drop")) {
        hardDrop();
    }
    // 暂停
    else if (m_inputManager->isActionJustPressed("Pause")) {
        m_state = GameState::PAUSED;
        m_selectedPauseItem = 0;
        if (m_uiManager) m_uiManager->setSelectedPauseItem(0);
    }
    // 返回菜单
    else if (m_inputManager->isActionJustPressed("Menu")) {
        m_state = GameState::MENU;
        m_leftPressed = false;
        m_rightPressed = false;
    }
}
void TetrisGame::handlePausedInput(sf::Keyboard::Key key) {
    // ========== 上键选择上一个菜单项 ==========
    if (key == sf::Keyboard::Key::Up) {
        m_selectedPauseItem = (m_selectedPauseItem - 1 + 3) % 3;
        if (m_uiManager) {
            m_uiManager->setSelectedPauseItem(m_selectedPauseItem);
        }
    }
    // ========== 下键选择下一个菜单项 ==========
    else if (key == sf::Keyboard::Key::Down) {
        m_selectedPauseItem = (m_selectedPauseItem + 1) % 3;
        if (m_uiManager) {
            m_uiManager->setSelectedPauseItem(m_selectedPauseItem);
        }
    }
    // ========== 回车确认选择 ==========
    else if (key == sf::Keyboard::Key::Enter) {
        switch (m_selectedPauseItem) {
        case 0:
            m_state = GameState::PLAYING;
            break;
        case 1:
            m_state = GameState::MENU;
            m_leftPressed = false;
            m_rightPressed = false;
            break;
        case 2:
            m_window.close();
            break;
        default:
            break;
        }
    }
    // ========== ESC 或 P 键继续游戏 ==========
    else if (key == sf::Keyboard::Key::Escape || key == sf::Keyboard::Key::P) {
        m_state = GameState::PLAYING;
    }
}