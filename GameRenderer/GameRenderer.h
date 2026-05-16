#pragma once
#include "raylib.h"
#include "../GameGrid/GameGrid.h"
#include "../Rules/Rules.h"

class GameRenderer {
public:
    GameRenderer(int screenWidth, int screenHeight, int cellSize);
    ~GameRenderer();
    
    void beginFrame();
    void endFrame();
    void drawWorld(const GameGrid& grid, bool isRunning);
    
    bool shouldClose() const;
    void handleZoom(int mouseWheelDelta);
    void handlePan(int deltaX, int deltaY);
    Vector2 getMouseCell() const;
    
    bool isPlayPausePressed() const;
    bool isStepPressed() const;
    bool isClearPressed() const;
    bool isRandomPressed() const;
    bool isRulesPressed() const;
    bool isColorAPressed() const;
    bool isColorBPressed() const;
    
    void nextColorA();
    void prevColorA();
    void nextColorB();
    void prevColorB();
    
    Cell::ColorIndex getDisplayColorA() const { return m_displayColorA; }
    Cell::ColorIndex getDisplayColorB() const { return m_displayColorB; }
    
private:
    int m_screenWidth, m_screenHeight;
    int m_cellSize;
    int m_offsetX, m_offsetY;
    Cell::ColorIndex m_displayColorA, m_displayColorB;
    Color m_palette[Cell::MAX_COLORS];
    
    struct Button { int x, y, width, height; };
    Button m_playPauseBtn;
    Button m_stepBtn;
    Button m_clearBtn;
    Button m_randomBtn;
    Button m_rulesBtn;
    Button m_colorASelector;
    Button m_colorBSelector;
    
    void initButtons();
    void initPalette();
    void drawButtons(bool isRunning);
    void drawColorSelectors();
    void drawCells(const GameGrid& grid);
    void drawGrid(const GameGrid& grid);
    bool isButtonPressed(const Button& btn) const;
};