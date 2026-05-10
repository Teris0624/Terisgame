#include "TetrisGame.h"
#include "tetromino.h"
#include "Constants.h"
#include <algorithm>
#include <vector>

// ========== 辅助函数：深拷贝形状 ==========
static void copyShape(int dest[4][4], const int src[4][4]) {
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            dest[i][j] = src[i][j];
        }
    }
}

// ========== 修正位置：将超出边界的方块推回地图内 ==========
void TetrisGame::fixPosition(Tetromino* piece, int& x, int& y) {
    if (!piece) return;
    int w = getCurrentWidth();
    int h = getCurrentHeight();

    // 获取方块的实际边界
    int minX = 4, maxX = -1, minY = 4, maxY = -1;
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            if (piece->shape[i][j] != 0) {
                minX = std::min(minX, j);
                maxX = std::max(maxX, j);
                minY = std::min(minY, i);
                maxY = std::max(maxY, i);
            }
        }
    }

    // 修正左边界：如果超出左边，向右推
    if (x + minX < 0) {
        x = -minX;
    }

    // 修正右边界：如果超出右边，向左推
    if (x + maxX >= w) {
        x = w - maxX - 1;
    }

    // 修正底部边界：如果超出底部，向上推
    if (y + maxY >= h) {
        y = h - maxY - 1;
    }

    // 顶部边界：允许超出（生成区域），不修正
    // 但如果超出太多，也适当修正
    if (y + minY < -4) {
        y = -minY - 2;
    }
}

// ========== 碰撞检测（支持超出边界的方块）==========
bool TetrisGame::checkCollision(const Tetromino* piece, int x, int y) {
    if (!piece || !m_mapManager) return true;
    int w = getCurrentWidth();
    int h = getCurrentHeight();

    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            if (piece->shape[i][j] != 0) {
                int gx = x + j;
                int gy = y + i;

                // 边界检查：如果超出边界过多，视为碰撞
                if (gx < -2 || gx >= w + 2) return true;
                if (gy < -4) return true;  // 顶部超出太多
                // 底部允许超出（不视为碰撞）

                // 只检测地图内的碰撞
                if (gx >= 0 && gx < w && gy >= 0 && gy < h) {
                    if (m_mapManager->getCell(gx, gy) != 0) return true;
                }
            }
        }
    }
    return false;
}

// ========== 旋转功能（带边界修正）==========
void TetrisGame::rotatePiece() {
    if (!m_currentPiece) return;

    // 保存原始状态
    int oldX = m_currentPiece->x;
    int oldY = m_currentPiece->y;
    int oldState = m_currentPiece->getCurrentState();
    int oldShape[4][4];
    copyShape(oldShape, m_currentPiece->shape);

    // 执行旋转
    m_currentPiece->rotate();

    // 修正位置（将超出边界的部分推回）
    int newX = oldX;
    int newY = oldY;
    fixPosition(m_currentPiece.get(), newX, newY);

    // 尝试位置序列
    int testX[] = { 0, -1, 1, 0, 0, -2, 2, -1, 1, -2, 2, -3, 3 };
    int testY[] = { 0, 0, 0, -1, 1, 0, 0, -1, 1, -1, 1, 0, 0 };
    int numTests = 13;

    for (int i = 0; i < numTests; i++) {
        int testXPos = newX + testX[i];
        int testYPos = newY + testY[i];

        // 修正测试位置
        fixPosition(m_currentPiece.get(), testXPos, testYPos);

        if (!checkCollision(m_currentPiece.get(), testXPos, testYPos)) {
            m_currentPiece->x = testXPos;
            m_currentPiece->y = testYPos;
            return;  // 旋转成功
        }
    }

    // 旋转失败，恢复原始状态
    m_currentPiece->x = oldX;
    m_currentPiece->y = oldY;
    copyShape(m_currentPiece->shape, oldShape);
    m_currentPiece->setCurrentState(oldState);
}

// ========== 移动方块 ==========
bool TetrisGame::movePiece(int dx, int dy) {
    if (!m_currentPiece) return false;
    int newX = m_currentPiece->x + dx;
    int newY = m_currentPiece->y + dy;

    // 修正位置
    fixPosition(m_currentPiece.get(), newX, newY);

    m_currentPiece->x = newX;
    m_currentPiece->y = newY;
    if (checkCollision(m_currentPiece.get(), m_currentPiece->x, m_currentPiece->y)) {
        m_currentPiece->x -= dx;
        m_currentPiece->y -= dy;
        return false;
    }
    return true;
}

// ========== 一键下落 ==========
void TetrisGame::hardDrop() {
    if (!m_currentPiece) return;

    // 循环下落直到碰撞
    while (true) {
        int newY = m_currentPiece->y + 1;
        fixPosition(m_currentPiece.get(), m_currentPiece->x, newY);

        if (checkCollision(m_currentPiece.get(), m_currentPiece->x, newY)) {
            break;  // 不能再下落
        }
        m_currentPiece->y = newY;
    }

    mergePiece();
    clearLines();
    spawnNewPiece();
}

// ========== 合并方块到地图 ==========
void TetrisGame::mergePiece() {
    if (!m_currentPiece || !m_mapManager) return;

    // 确保位置有效
    fixPosition(m_currentPiece.get(), m_currentPiece->x, m_currentPiece->y);

    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            if (m_currentPiece->shape[i][j] != 0) {
                int gx = m_currentPiece->x + j;
                int gy = m_currentPiece->y + i;
                // 只合并地图范围内的方块
                if (gy >= 0 && gy < getCurrentHeight() && gx >= 0 && gx < getCurrentWidth()) {
                    if (m_mapManager->getCell(gx, gy) == 0) {
                        m_mapManager->setCell(gx, gy, m_currentPiece->type + 1);
                    }
                }
            }
        }
    }
}

// ========== 消除满行 ==========
void TetrisGame::clearLines() {
    if (!m_mapManager) return;
    int lines = 0;
    int h = getCurrentHeight();
    int w = getCurrentWidth();

    for (int row = h - 1; row >= 0; ) {
        bool full = true;
        for (int col = 0; col < w; ++col) {
            if (m_mapManager->getCell(col, row) == 0) {
                full = false;
                break;
            }
        }
        if (full) {
            for (int r = row; r > 0; --r) {
                for (int c = 0; c < w; ++c) {
                    m_mapManager->setCell(c, r, m_mapManager->getCell(c, r - 1));
                }
            }
            for (int c = 0; c < w; ++c) {
                m_mapManager->setCell(c, 0, 0);
            }
            ++lines;
        }
        else {
            --row;
        }
    }
    if (lines > 0) {
        updateScore(lines);
    }
}

// ========== 更新分数 ==========
void TetrisGame::updateScore(int lines) {
    if (!m_settingsManager) return;
    const int SCORES[] = { 0, 100, 300, 500, 800 };
    int idx = std::min(lines, 4);
    GameSettings s = m_settingsManager->getSettings();
    m_score += SCORES[idx] * m_level * s.scoreMultiplier;
    m_linesCleared += lines;

    int newLevel = m_linesCleared / 10 + 1;
    if (newLevel > m_level) {
        m_level = newLevel;
        m_fallSpeed = s.initialFallSpeed / (1.0f + static_cast<float>(m_level - 1) * 0.1f);
        if (m_fallSpeed < 0.08f) m_fallSpeed = 0.08f;
        if (m_mapManager) {
            m_mapManager->createLevel(m_level);
        }
    }
}

// ========== 生成新方块 ==========
void TetrisGame::spawnNewPiece() {
    if (!m_mapManager) return;

    m_currentPiece = std::move(m_nextPiece);
    m_nextPiece = std::make_unique<Tetromino>(m_dist(m_rng));

    if (!m_currentPiece) return;

    // 重置到原始状态
    m_currentPiece->resetToState0();

    // 计算生成位置
    int startX = getCurrentWidth() / 2 - 2;
    startX = std::max(0, std::min(startX, getCurrentWidth() - 4));
    m_currentPiece->x = startX;
    m_currentPiece->y = -1;  // 从顶部外开始生成

    // 修正位置
    fixPosition(m_currentPiece.get(), m_currentPiece->x, m_currentPiece->y);

    if (checkCollision(m_currentPiece.get(), m_currentPiece->x, m_currentPiece->y)) {
        m_state = GameState::GAME_OVER;
    }
}

// ========== 重置游戏 ==========
void TetrisGame::resetGame() {
    if (!m_settingsManager || !m_mapManager) return;
    GameSettings s = m_settingsManager->getSettings();
    m_mapManager->resize(s.gridWidth, s.gridHeight);
    m_mapManager->clear();
    m_state = GameState::PLAYING;
    m_score = 0;
    m_level = 1;
    m_linesCleared = 0;
    m_fallSpeed = s.initialFallSpeed;
    m_fallTimer = 0.0f;
    m_leftPressed = false;
    m_rightPressed = false;
    m_moveDelay = 0.0f;
    m_selectedPauseItem = 0;
    if (m_uiManager) m_uiManager->setSelectedPauseItem(0);

    m_currentPiece = std::make_unique<Tetromino>(m_dist(m_rng));
    m_nextPiece = std::make_unique<Tetromino>(m_dist(m_rng));
    if (m_currentPiece) {
        m_currentPiece->resetToState0();
        int startX = getCurrentWidth() / 2 - 2;
        startX = std::max(0, std::min(startX, getCurrentWidth() - 4));
        m_currentPiece->x = startX;
        m_currentPiece->y = -1;
        fixPosition(m_currentPiece.get(), m_currentPiece->x, m_currentPiece->y);
    }
}

// ========== 更新函数 ==========
void TetrisGame::update(float dt) {
    if (m_state == GameState::MENU) {
        m_titleBobTimer += dt * 3.0f;
        m_titleBobOffset = std::sin(m_titleBobTimer) * 10.0f;
    }

    if (m_state != GameState::PLAYING || !m_currentPiece) return;

    if (m_leftPressed || m_rightPressed) {
        m_moveDelay -= dt;
        if (m_moveDelay <= 0.0f) {
            if (m_leftPressed) movePiece(-1, 0);
            if (m_rightPressed) movePiece(1, 0);
            m_moveDelay = m_moveRepeatInterval;
        }
    }

    m_fallTimer += dt;
    if (m_fallTimer >= m_fallSpeed) {
        m_fallTimer = 0.0f;
        movePiece(0, 1);
    }
}