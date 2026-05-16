#pragma once
#include "../GameGrid/GameGrid.h"
#include "../GameRenderer/GameRenderer.h"
#include "../Rules/Rules.h"

class Game {
public:
    Game(int width, int height, int cellSize = 6);
    void run();
private:
    GameGrid m_grid;
    GameRenderer m_renderer;
    Rules m_rules;
    bool m_running;
    bool m_autoUpdate;
    int m_updateDelayMs;
    
    bool m_settingsOpen;
    char m_neighborInputAB[64];
    char m_neighborInputBA[64];
    float m_probAB;
    float m_probBA;
    
    void handleInput();
    void updateSimulation();
    void drawSettingsWindow();
    void applyRulesToCurrentColors();
};