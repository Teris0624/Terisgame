#include "UIManager.h"
#include "Tetromino.h"
#include "Constants.h"

void UIManager::drawGameUI(int score, int level, int lines, int currentWidth,
    const sf::Color& nextColor, const int nextShape[4][4]) {
    sf::Vector2u size = m_window.getSize();
    int cell = getScaledCellSize();

    int gameAreaW = currentWidth * cell;
    int sidebarW = static_cast<int>(static_cast<float>(size.x) * 0.28f);
    sidebarW = std::max(150, std::min(sidebarW, 400));

    if (gameAreaW + sidebarW > static_cast<int>(size.x)) {
        sidebarW = static_cast<int>(size.x) - gameAreaW - 20;
        sidebarW = std::max(120, sidebarW);
    }

    // 侧边栏背景
    sf::RectangleShape sidebar(sf::Vector2f(static_cast<float>(sidebarW), static_cast<float>(size.y)));
    sidebar.setFillColor(sf::Color(25, 25, 45));
    sidebar.setPosition(sf::Vector2f(static_cast<float>(gameAreaW), 0.0f));
    m_window.draw(sidebar);

    // 分隔线
    sf::Vertex div[] = {
        sf::Vertex(sf::Vector2f(static_cast<float>(gameAreaW), 0.0f), sf::Color(60,60,100)),
        sf::Vertex(sf::Vector2f(static_cast<float>(gameAreaW), static_cast<float>(size.y)), sf::Color(60,60,100))
    };
    m_window.draw(div, 2, sf::PrimitiveType::Lines);

    float infoX = static_cast<float>(gameAreaW) + getScaledW(20.0f);
    float contentW = static_cast<float>(sidebarW) - getScaledW(40.0f);

    // 得分
    sf::Text scoreT(m_font, "Score: " + std::to_string(score), getFontSize(22));
    scoreT.setFillColor(UI_TEXT_COLOR);
    scoreT.setPosition(sf::Vector2f(infoX, getScaledY(50.0f)));
    m_window.draw(scoreT);

    // 等级
    sf::Text levelT(m_font, "Level: " + std::to_string(level), getFontSize(22));
    levelT.setFillColor(UI_TEXT_COLOR);
    levelT.setPosition(sf::Vector2f(infoX, getScaledY(100.0f)));
    m_window.draw(levelT);

    // 消除行数
    sf::Text linesT(m_font, "Lines: " + std::to_string(lines), getFontSize(20));
    linesT.setFillColor(UI_TEXT_COLOR);
    linesT.setPosition(sf::Vector2f(infoX, getScaledY(150.0f)));
    m_window.draw(linesT);

    // 下一个方块标题
    sf::Text nextTitle(m_font, "Next", getFontSize(18));
    nextTitle.setFillColor(UI_TEXT_COLOR);
    nextTitle.setPosition(sf::Vector2f(infoX, getScaledY(210.0f)));
    m_window.draw(nextTitle);

    // 预览区域
    int previewSize = cell * 4;
    int previewX = gameAreaW + (sidebarW - previewSize) / 2;
    int previewY = static_cast<int>(getScaledY(250.0f));

    sf::RectangleShape previewBg(sf::Vector2f(static_cast<float>(previewSize + 10), static_cast<float>(previewSize + 10)));
    previewBg.setFillColor(sf::Color(30, 30, 50));
    previewBg.setOutlineColor(sf::Color(60, 60, 100));
    previewBg.setOutlineThickness(2.0f);
    previewBg.setPosition(sf::Vector2f(static_cast<float>(previewX - 5), static_cast<float>(previewY - 5)));
    m_window.draw(previewBg);

    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            if (nextShape[i][j] != 0) {
                sf::RectangleShape rect(sf::Vector2f(static_cast<float>(cell - 2), static_cast<float>(cell - 2)));
                rect.setPosition(sf::Vector2f(
                    static_cast<float>(previewX + j * cell),
                    static_cast<float>(previewY + i * cell)
                ));
                rect.setFillColor(nextColor);
                m_window.draw(rect);
            }
        }
    }

    // 控制面板
    float panelY = static_cast<float>(size.y) - getScaledH(180.0f);
    float panelH = getScaledH(160.0f);

    sf::RectangleShape panel(sf::Vector2f(contentW, panelH));
    panel.setFillColor(sf::Color(0, 0, 0, 150));
    panel.setOutlineColor(UI_HINT_COLOR);
    panel.setOutlineThickness(1.0f);
    panel.setPosition(sf::Vector2f(infoX, panelY));
    m_window.draw(panel);

    sf::Text panelTitle(m_font, "CONTROLS", getFontSize(12));
    panelTitle.setFillColor(UI_TITLE_COLOR);
    panelTitle.setPosition(sf::Vector2f(infoX + 10.0f, panelY + 5.0f));
    m_window.draw(panelTitle);

    sf::RectangleShape sep(sf::Vector2f(contentW - 20.0f, 1.0f));
    sep.setFillColor(sf::Color(100, 100, 100));
    sep.setPosition(sf::Vector2f(infoX + 10.0f, panelY + getScaledH(18.0f)));
    m_window.draw(sep);

    float yOff = panelY + getScaledH(22.0f);
    float keyX = infoX + 15.0f;
    float actionX = infoX + contentW * 0.45f;
    int ctrlFont = getFontSize(10);

    std::vector<std::pair<std::string, std::string>> controls = {
        {"Q / E", "Move"}, {"S", "Down"}, {"W", "Rotate"},
        {"Space", "Drop"}, {"P", "Pause"}, {"ESC", "Menu"}
    };

    for (const auto& c : controls) {
        sf::Text key(m_font, c.first, ctrlFont);
        key.setFillColor(UI_HINT_COLOR);
        key.setPosition(sf::Vector2f(keyX, yOff));
        m_window.draw(key);

        sf::Text act(m_font, c.second, ctrlFont);
        act.setFillColor(sf::Color(200, 200, 200));
        act.setPosition(sf::Vector2f(actionX, yOff));
        m_window.draw(act);

        yOff += getScaledH(16.0f);
    }
}

void UIManager::drawGameArea(int grid[20][16], int width, int height,
    const Tetromino* piece, bool showGhost) {
    sf::Vector2u size = m_window.getSize();
    int cell = getScaledCellSize();

    int gameAreaW = width * cell;
    int gameAreaH = height * cell;

    if (gameAreaH > static_cast<int>(size.y)) {
        int newCell = static_cast<int>(static_cast<float>(size.y) / static_cast<float>(height)) - 2;
        if (newCell >= 15) {
            cell = newCell;
            gameAreaW = width * cell;
            gameAreaH = height * cell;
        }
    }

    // 背景
    sf::RectangleShape bg(sf::Vector2f(static_cast<float>(gameAreaW), static_cast<float>(gameAreaH)));
    bg.setFillColor(sf::Color(10, 10, 30));
    bg.setPosition(sf::Vector2f(0.0f, 0.0f));
    m_window.draw(bg);

    // 网格线
    for (int x = 0; x <= width; ++x) {
        float xPos = static_cast<float>(x * cell);
        sf::Vertex line[] = {
            sf::Vertex(sf::Vector2f(xPos, 0.0f), sf::Color(40, 40, 60)),
            sf::Vertex(sf::Vector2f(xPos, static_cast<float>(gameAreaH)), sf::Color(40, 40, 60))
        };
        m_window.draw(line, 2, sf::PrimitiveType::Lines);
    }
    for (int y = 0; y <= height; ++y) {
        float yPos = static_cast<float>(y * cell);
        sf::Vertex line[] = {
            sf::Vertex(sf::Vector2f(0.0f, yPos), sf::Color(40, 40, 60)),
            sf::Vertex(sf::Vector2f(static_cast<float>(gameAreaW), yPos), sf::Color(40, 40, 60))
        };
        m_window.draw(line, 2, sf::PrimitiveType::Lines);
    }

    // 固定方块
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            int val = grid[i][j];
            if (val > 0) {
                sf::RectangleShape rect(sf::Vector2f(static_cast<float>(cell - 1), static_cast<float>(cell - 1)));
                rect.setPosition(sf::Vector2f(static_cast<float>(j * cell), static_cast<float>(i * cell)));
                rect.setFillColor(COLORS[val % 8]);
                m_window.draw(rect);
            }
        }
    }

    // 幽灵方块
    if (showGhost && piece) {
        int ghostY = piece->y;
        while (true) {
            bool canMove = true;
            for (int i = 0; i < 4 && canMove; ++i) {
                for (int j = 0; j < 4 && canMove; ++j) {
                    if (piece->shape[i][j] != 0) {
                        int gx = piece->x + j;
                        int gy = ghostY + i + 1;
                        if (gx < 0 || gx >= width || gy >= height || (gy >= 0 && grid[gy][gx] != 0)) {
                            canMove = false;
                        }
                    }
                }
            }
            if (!canMove) break;
            ghostY++;
        }

        if (ghostY != piece->y) {
            for (int i = 0; i < 4; ++i) {
                for (int j = 0; j < 4; ++j) {
                    if (piece->shape[i][j] != 0) {
                        sf::RectangleShape rect(sf::Vector2f(static_cast<float>(cell - 1), static_cast<float>(cell - 1)));
                        rect.setPosition(sf::Vector2f(
                            static_cast<float>((piece->x + j) * cell),
                            static_cast<float>((ghostY + i) * cell)
                        ));
                        rect.setFillColor(sf::Color(255, 255, 255, 100));
                        m_window.draw(rect);
                    }
                }
            }
        }
    }

    // 当前方块
    if (piece) {
        for (int i = 0; i < 4; ++i) {
            for (int j = 0; j < 4; ++j) {
                if (piece->shape[i][j] != 0) {
                    sf::RectangleShape rect(sf::Vector2f(static_cast<float>(cell - 1), static_cast<float>(cell - 1)));
                    rect.setPosition(sf::Vector2f(
                        static_cast<float>((piece->x + j) * cell),
                        static_cast<float>((piece->y + i) * cell)
                    ));
                    rect.setFillColor(piece->color);
                    m_window.draw(rect);
                }
            }
        }
    }
}