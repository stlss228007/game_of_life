#include "GameGrid.h"
#include <chrono>
#include <cstdlib>
#include <ctime>

GameGrid::GameGrid(int width, int height) 
    : m_width(width), m_height(height), 
      m_rng(std::chrono::steady_clock::now().time_since_epoch().count()),
      m_randProb(0.0f, 1.0f) {
    resize(width, height);
    std::srand(static_cast<unsigned>(std::time(nullptr)));
}

void GameGrid::resize(int width, int height) {
    m_width = width;
    m_height = height;
    m_grid.assign(height, std::vector<Cell>(width, Cell(1)));
    m_nextGrid.assign(height, std::vector<Cell>(width, Cell(1)));
}

void GameGrid::setRules(const Rules& rules) {
    m_rules = rules;
}

void GameGrid::update() {
    for (int y = 0; y < m_height; ++y) {
        for (int x = 0; x < m_width; ++x) {
            m_nextGrid[y][x].setColor(calculateNextState(x, y));
        }
    }
    m_grid.swap(m_nextGrid);
}

void GameGrid::setCellColor(int x, int y, ColorIndex color) {
    if (isInBounds(x, y)) m_grid[y][x].setColor(color);
}

GameGrid::ColorIndex GameGrid::getCellColor(int x, int y) const {
    if (!isInBounds(x, y)) return 1;
    return m_grid[y][x].getColor();
}

int GameGrid::getWidth() const { return m_width; }
int GameGrid::getHeight() const { return m_height; }

void GameGrid::fill(ColorIndex color) {
    for (int y = 0; y < m_height; ++y)
        for (int x = 0; x < m_width; ++x)
            m_grid[y][x].setColor(color);
}

void GameGrid::randomizeWithTwoColors(ColorIndex colorA, ColorIndex colorB, float probA) {
    std::uniform_real_distribution<float> dist(0.0f, 1.0f);
    for (int y = 0; y < m_height; ++y) {
        for (int x = 0; x < m_width; ++x) {
            float r = dist(m_rng);
            m_grid[y][x].setColor(r < probA ? colorA : colorB);
        }
    }
}

int GameGrid::countSameColorNeighbors(int x, int y, ColorIndex color) const {
    int count = 0;
    for (int dy = -1; dy <= 1; ++dy) {
        for (int dx = -1; dx <= 1; ++dx) {
            if (dx == 0 && dy == 0) continue;
            int nx = x + dx;
            int ny = y + dy;
            if (isInBounds(nx, ny) && m_grid[ny][nx].getColor() == color)
                ++count;
        }
    }
    return count;
}

GameGrid::ColorIndex GameGrid::calculateNextState(int x, int y) {
    ColorIndex current = m_grid[y][x].getColor();
    const auto& rules = m_rules.getRulesForColor(current);
    for (const auto& rule : rules) {
        bool matches = false;
        for (int n : rule.neighborCounts) {
            if (countSameColorNeighbors(x, y, current) == n) {
                matches = true;
                break;
            }
        }
        if (matches) {
            if (m_randProb(m_rng) < rule.probability) {
                return rule.toColor;
            } else {
                return current;
            }
        }
    }
    return current;
}

bool GameGrid::isInBounds(int x, int y) const {
    return x >= 0 && x < m_width && y >= 0 && y < m_height;
}