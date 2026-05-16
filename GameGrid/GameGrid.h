#pragma once
#include "../Cell/Cell.h"
#include "../Rules/Rules.h"
#include <vector>
#include <random>

class GameGrid {
public:
    using ColorIndex = Cell::ColorIndex;
    
    GameGrid(int width, int height);
    void resize(int width, int height);
    void setRules(const Rules& rules);
    void update();
    void setCellColor(int x, int y, ColorIndex color);
    ColorIndex getCellColor(int x, int y) const;
    int getWidth() const;
    int getHeight() const;
    void fill(ColorIndex color = 1);
    void randomizeWithTwoColors(ColorIndex colorA, ColorIndex colorB, float probA = 0.5f);
    
private:
    int m_width, m_height;
    std::vector<std::vector<Cell>> m_grid;
    std::vector<std::vector<Cell>> m_nextGrid;
    Rules m_rules;
    std::mt19937 m_rng;
    mutable std::uniform_real_distribution<float> m_randProb;
    
    int countSameColorNeighbors(int x, int y, ColorIndex color) const;
    ColorIndex calculateNextState(int x, int y);
    bool isInBounds(int x, int y) const;
};