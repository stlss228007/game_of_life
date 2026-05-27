#include "GameRenderer.h"

GameRenderer::GameRenderer(int screenWidth, int screenHeight, int cellSize)
    : m_screenWidth(screenWidth), m_screenHeight(screenHeight), m_cellSize(cellSize), m_offsetX(0), m_offsetY(0),
      m_displayColorA(1), m_displayColorB(2) {
    InitWindow(screenWidth, screenHeight, "Multi-Color Cellular Automaton");
    SetTargetFPS(60);
    initPalette();
    initButtons();
}

GameRenderer::~GameRenderer() {
    CloseWindow();
}

void GameRenderer::initPalette() {
    m_palette[0] = BLACK;
    m_palette[1] = RED;
    m_palette[2] = GREEN;
    m_palette[3] = BLUE;
    m_palette[4] = YELLOW;
    m_palette[5] = MAGENTA;
    m_palette[6] = SKYBLUE;
    m_palette[7] = ORANGE;
    m_palette[8] = PURPLE;
    m_palette[9] = PINK;
}

void GameRenderer::initButtons() {
    int btnW = 80, btnH = 30;
    int startX = 10;
    int y = m_screenHeight - btnH - 10;
    m_playPauseBtn = {startX, y, btnW, btnH};
    m_stepBtn = {startX + btnW + 5, y, btnW, btnH};
    m_clearBtn = {startX + 2*(btnW + 5), y, btnW, btnH};
    m_randomBtn = {startX + 3*(btnW + 5), y, btnW, btnH};
    m_rulesBtn = {startX + 4*(btnW + 5), y, btnW, btnH};
    m_colorASelector = {startX + 5*(btnW + 5), y, 60, btnH};
    m_colorBSelector = {startX + 6*(btnW + 5), y, 60, btnH};
}

void GameRenderer::beginFrame() {
    BeginDrawing();
    ClearBackground(DARKGRAY);
}

void GameRenderer::endFrame() {
    EndDrawing();
}

void GameRenderer::drawWorld(const GameGrid& grid, bool isRunning) {
    drawCells(grid);
    drawGrid(grid);
    drawButtons(isRunning);
    drawColorSelectors();
}

void GameRenderer::drawButtons(bool isRunning) {
    DrawRectangle(m_playPauseBtn.x, m_playPauseBtn.y, m_playPauseBtn.width, m_playPauseBtn.height, LIGHTGRAY);
    DrawRectangleLines(m_playPauseBtn.x, m_playPauseBtn.y, m_playPauseBtn.width, m_playPauseBtn.height, DARKGRAY);
    DrawText(isRunning ? "Pause" : "Play", m_playPauseBtn.x + 10, m_playPauseBtn.y + 8, 16, BLACK);
    
    DrawRectangle(m_stepBtn.x, m_stepBtn.y, m_stepBtn.width, m_stepBtn.height, LIGHTGRAY);
    DrawRectangleLines(m_stepBtn.x, m_stepBtn.y, m_stepBtn.width, m_stepBtn.height, DARKGRAY);
    DrawText("Step", m_stepBtn.x + 15, m_stepBtn.y + 8, 16, BLACK);
    
    DrawRectangle(m_clearBtn.x, m_clearBtn.y, m_clearBtn.width, m_clearBtn.height, LIGHTGRAY);
    DrawRectangleLines(m_clearBtn.x, m_clearBtn.y, m_clearBtn.width, m_clearBtn.height, DARKGRAY);
    DrawText("Clear", m_clearBtn.x + 12, m_clearBtn.y + 8, 16, BLACK);
    
    DrawRectangle(m_randomBtn.x, m_randomBtn.y, m_randomBtn.width, m_randomBtn.height, LIGHTGRAY);
    DrawRectangleLines(m_randomBtn.x, m_randomBtn.y, m_randomBtn.width, m_randomBtn.height, DARKGRAY);
    DrawText("Random", m_randomBtn.x + 8, m_randomBtn.y + 8, 16, BLACK);
    
    DrawRectangle(m_rulesBtn.x, m_rulesBtn.y, m_rulesBtn.width, m_rulesBtn.height, LIGHTGRAY);
    DrawRectangleLines(m_rulesBtn.x, m_rulesBtn.y, m_rulesBtn.width, m_rulesBtn.height, DARKGRAY);
    DrawText("Rules", m_rulesBtn.x + 12, m_rulesBtn.y + 8, 16, BLACK);
}

void GameRenderer::drawColorSelectors() {
    DrawRectangle(m_colorASelector.x, m_colorASelector.y, m_colorASelector.width, m_colorASelector.height, LIGHTGRAY);
    DrawRectangleLines(m_colorASelector.x, m_colorASelector.y, m_colorASelector.width, m_colorASelector.height, DARKGRAY);
    DrawText(TextFormat("A:%d", m_displayColorA), m_colorASelector.x + 5, m_colorASelector.y + 8, 14, BLACK);
    
    DrawRectangle(m_colorBSelector.x, m_colorBSelector.y, m_colorBSelector.width, m_colorBSelector.height, LIGHTGRAY);
    DrawRectangleLines(m_colorBSelector.x, m_colorBSelector.y, m_colorBSelector.width, m_colorBSelector.height, DARKGRAY);
    DrawText(TextFormat("B:%d", m_displayColorB), m_colorBSelector.x + 5, m_colorBSelector.y + 8, 14, BLACK);
}

void GameRenderer::drawCells(const GameGrid& grid) {
    int startX = m_offsetX / m_cellSize;
    int startY = m_offsetY / m_cellSize;
    int endX = startX + m_screenWidth / m_cellSize + 2;
    int endY = startY + m_screenHeight / m_cellSize + 2;
    for (int y = startY; y < endY; ++y) {
        for (int x = startX; x < endX; ++x) {
            if (x >= 0 && x < grid.getWidth() && y >= 0 && y < grid.getHeight()) {
                Cell::ColorIndex color = grid.getCellColor(x, y);
                // Убираем условие на m_displayColorA/B – рисуем все цвета
                if (color >= 1 && color < Cell::MAX_COLORS) {
                    Color drawColor = m_palette[color];
                    int screenX = x * m_cellSize - m_offsetX;
                    int screenY = y * m_cellSize - m_offsetY;
                    DrawRectangle(screenX, screenY, m_cellSize - 1, m_cellSize - 1, drawColor);
                }
            }
        }
    }
}

void GameRenderer::drawGrid(const GameGrid& grid) {
    int leftBound   = -m_offsetX;
    int rightBound  = grid.getWidth() * m_cellSize - m_offsetX;
    int topBound    = -m_offsetY;
    int bottomBound = grid.getHeight() * m_cellSize - m_offsetY;

    int startY = std::max(0, m_offsetY / m_cellSize);
    int endY   = std::min(grid.getHeight(), startY + m_screenHeight / m_cellSize + 2);
    for (int y = startY; y <= endY; ++y) {
        int screenY = y * m_cellSize - m_offsetY;
        if (screenY < 0 || screenY > m_screenHeight) continue;
        int lineStartX = std::max(0, leftBound);
        int lineEndX   = std::min(m_screenWidth, rightBound);
        if (lineStartX < lineEndX) {
            DrawLine(lineStartX, screenY, lineEndX, screenY, LIGHTGRAY);
        }
    }

    int startX = std::max(0, m_offsetX / m_cellSize);
    int endX   = std::min(grid.getWidth(), startX + m_screenWidth / m_cellSize + 2);
    for (int x = startX; x <= endX; ++x) {
        int screenX = x * m_cellSize - m_offsetX;
        if (screenX < 0 || screenX > m_screenWidth) continue;
        int lineStartY = std::max(0, topBound);
        int lineEndY   = std::min(m_screenHeight, bottomBound);
        if (lineStartY < lineEndY) {
            DrawLine(screenX, lineStartY, screenX, lineEndY, LIGHTGRAY);
        }
    }
}

bool GameRenderer::shouldClose() const {
    return WindowShouldClose();
}

void GameRenderer::handleZoom(int mouseWheelDelta) {
    int newSize = m_cellSize + mouseWheelDelta;
    if (newSize >= 5 && newSize <= 100) {
        Vector2 mouse = GetMousePosition();
        float oldSize = m_cellSize;
        m_cellSize = newSize;
        m_offsetX = static_cast<int>(mouse.x - (mouse.x + m_offsetX) * m_cellSize / oldSize);
        m_offsetY = static_cast<int>(mouse.y - (mouse.y + m_offsetY) * m_cellSize / oldSize);
    }
}

void GameRenderer::handlePan(int deltaX, int deltaY) {
    m_offsetX += deltaX;
    m_offsetY += deltaY;
    if (m_offsetX < 0) m_offsetX = 0;
    if (m_offsetY < 0) m_offsetY = 0;
}

Vector2 GameRenderer::getMouseCell() const {
    Vector2 mouse = GetMousePosition();
    int gridX = (static_cast<int>(mouse.x) + m_offsetX) / m_cellSize;
    int gridY = (static_cast<int>(mouse.y) + m_offsetY) / m_cellSize;
    return {static_cast<float>(gridX), static_cast<float>(gridY)};
}

bool GameRenderer::isButtonPressed(const Button& btn) const {
    Vector2 mouse = GetMousePosition();
    return IsMouseButtonPressed(MOUSE_BUTTON_LEFT) &&
           mouse.x >= btn.x && mouse.x <= btn.x + btn.width &&
           mouse.y >= btn.y && mouse.y <= btn.y + btn.height;
}

bool GameRenderer::isPlayPausePressed() const { return isButtonPressed(m_playPauseBtn); }
bool GameRenderer::isStepPressed() const { return isButtonPressed(m_stepBtn); }
bool GameRenderer::isClearPressed() const { return isButtonPressed(m_clearBtn); }
bool GameRenderer::isRandomPressed() const { return isButtonPressed(m_randomBtn); }
bool GameRenderer::isRulesPressed() const { return isButtonPressed(m_rulesBtn); }
bool GameRenderer::isColorAPressed() const { return isButtonPressed(m_colorASelector); }
bool GameRenderer::isColorBPressed() const { return isButtonPressed(m_colorBSelector); }

void GameRenderer::nextColorA() {
    m_displayColorA = (m_displayColorA % (Cell::MAX_COLORS-1)) + 1;
}
void GameRenderer::prevColorA() {
    m_displayColorA = (m_displayColorA == 1) ? (Cell::MAX_COLORS-1) : (m_displayColorA - 1);
}
void GameRenderer::nextColorB() {
    m_displayColorB = (m_displayColorB % (Cell::MAX_COLORS-1)) + 1;
}
void GameRenderer::prevColorB() {
    m_displayColorB = (m_displayColorB == 1) ? (Cell::MAX_COLORS-1) : (m_displayColorB - 1);
}