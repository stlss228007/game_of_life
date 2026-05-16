#include "Game.h"
#include "raylib.h"
#include "raygui.h"
#include <iostream>
#include <chrono>
#include <thread>
#include <sstream>
#include <cstring>
#include <cstdio>

Game::Game(int width, int height, int cellSize)
    : m_grid(width, height), m_renderer(1280, 720, cellSize), 
      m_running(true), m_autoUpdate(true), m_updateDelayMs(100),
      m_settingsOpen(false), m_probAB(0.95f), m_probBA(1.0f) {
    
    strcpy(m_neighborInputAB, "4 6 7 8");
    strcpy(m_neighborInputBA, "0 1 2 4");
    
    applyRulesToCurrentColors();
    m_grid.randomizeWithTwoColors(m_renderer.getDisplayColorA(), m_renderer.getDisplayColorB(), 0.5f);
}

void Game::applyRulesToCurrentColors() {
    Cell::ColorIndex A = m_renderer.getDisplayColorA();
    Cell::ColorIndex B = m_renderer.getDisplayColorB();
    
    // Удалить старые правила для A и B
    m_rules.clearRulesForColor(A);
    m_rules.clearRulesForColor(B);
    
    // Правило A -> B
    std::vector<int> countsAB;
    std::stringstream ssAB(m_neighborInputAB);
    int val;
    while (ssAB >> val) countsAB.push_back(val);
    TransitionRule ruleAB;
    ruleAB.fromColor = A;
    ruleAB.neighborCounts = countsAB;
    ruleAB.toColor = B;
    ruleAB.probability = m_probAB;
    m_rules.addRule(ruleAB);
    
    // Правило B -> A
    std::vector<int> countsBA;
    std::stringstream ssBA(m_neighborInputBA);
    while (ssBA >> val) countsBA.push_back(val);
    TransitionRule ruleBA;
    ruleBA.fromColor = B;
    ruleBA.neighborCounts = countsBA;
    ruleBA.toColor = A;
    ruleBA.probability = m_probBA;
    m_rules.addRule(ruleBA);
    
    m_grid.setRules(m_rules);
}

void Game::drawSettingsWindow() {
    DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), Color{0,0,0,150});
    Rectangle winRect = {200, 100, 500, 400};
    GuiWindowBox(winRect, "Transition Rules Editor");
    
    Cell::ColorIndex A = m_renderer.getDisplayColorA();
    Cell::ColorIndex B = m_renderer.getDisplayColorB();
    char titleAB[64];
    snprintf(titleAB, 64, "Color %d -> Color %d", A, B);
    GuiLabel({210, 140, 300, 20}, titleAB);
    GuiLabel({210, 170, 100, 20}, "Neighbors:");
    GuiTextBox({320, 170, 200, 24}, m_neighborInputAB, 64, true);
    GuiLabel({210, 210, 100, 20}, "Probability:");
    GuiSlider({320, 210, 200, 20}, "0.0", "1.0", &m_probAB, 0.0f, 1.0f);
    char probText[16];
    snprintf(probText, 16, "%.2f", m_probAB);
    GuiLabel({530, 210, 40, 20}, probText);
    
    char titleBA[64];
    snprintf(titleBA, 64, "Color %d -> Color %d", B, A);
    GuiLabel({210, 260, 300, 20}, titleBA);
    GuiLabel({210, 290, 100, 20}, "Neighbors:");
    GuiTextBox({320, 290, 200, 24}, m_neighborInputBA, 64, true);
    GuiLabel({210, 330, 100, 20}, "Probability:");
    GuiSlider({320, 330, 200, 20}, "0.0", "1.0", &m_probBA, 0.0f, 1.0f);
    snprintf(probText, 16, "%.2f", m_probBA);
    GuiLabel({530, 330, 40, 20}, probText);
    
    if (GuiButton({250, 370, 100, 30}, "Save")) {
        applyRulesToCurrentColors();
        m_settingsOpen = false;
    }
    if (GuiButton({400, 370, 100, 30}, "Cancel")) {
        // Откат не требуется, т.к. изменения не применялись
        m_settingsOpen = false;
    }
}

void Game::run() {
    auto lastUpdate = std::chrono::steady_clock::now();
    while (!m_renderer.shouldClose() && m_running) {
        handleInput();
        auto now = std::chrono::steady_clock::now();
        if (!m_settingsOpen && m_autoUpdate && 
            std::chrono::duration_cast<std::chrono::milliseconds>(now - lastUpdate).count() >= m_updateDelayMs) {
            updateSimulation();
            lastUpdate = now;
        }
        m_renderer.beginFrame();
        m_renderer.drawWorld(m_grid, m_autoUpdate);
        if (m_settingsOpen) {
            drawSettingsWindow();
        }
        m_renderer.endFrame();
    }
}

void Game::handleInput() {
    if (m_settingsOpen) return;
    
    if (m_renderer.isPlayPausePressed()) m_autoUpdate = !m_autoUpdate;
    if (m_renderer.isStepPressed() && !m_autoUpdate) updateSimulation();
    if (m_renderer.isClearPressed()) m_grid.fill(m_renderer.getDisplayColorA());
    if (m_renderer.isRandomPressed()) {
        m_grid.randomizeWithTwoColors(m_renderer.getDisplayColorA(), m_renderer.getDisplayColorB(), 0.5f);
    }
    if (m_renderer.isRulesPressed()) {
        m_settingsOpen = true;
    }
    
    // Переключение отображаемых цветов: обновляем правила для новой пары
    if (m_renderer.isColorAPressed()) {
        m_renderer.nextColorA();
        applyRulesToCurrentColors();
        // Не меняем состояние поля
    }
    if (m_renderer.isColorBPressed()) {
        m_renderer.nextColorB();
        applyRulesToCurrentColors();
    }
    
    int wheel = GetMouseWheelMove();
    if (wheel != 0) m_renderer.handleZoom(wheel);
    if (IsKeyPressed(KEY_SPACE)) m_autoUpdate = !m_autoUpdate;
    if (IsKeyPressed(KEY_R)) {
        m_grid.randomizeWithTwoColors(m_renderer.getDisplayColorA(), m_renderer.getDisplayColorB(), 0.5f);
    }
    if (IsKeyPressed(KEY_C)) m_grid.fill(m_renderer.getDisplayColorA());
    if (IsKeyPressed(KEY_N) && !m_autoUpdate) updateSimulation();
    if (IsKeyPressed(KEY_ESCAPE)) m_running = false;
    if (IsKeyPressed(KEY_S)) {
        m_settingsOpen = true;
    }
    
    if (IsKeyDown(KEY_LEFT)) m_renderer.handlePan(-10, 0);
    if (IsKeyDown(KEY_RIGHT)) m_renderer.handlePan(10, 0);
    if (IsKeyDown(KEY_UP)) m_renderer.handlePan(0, -10);
    if (IsKeyDown(KEY_DOWN)) m_renderer.handlePan(0, 10);
    
    // Левый клик ставит цвет A, правый – цвет B
    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && !m_renderer.isPlayPausePressed() && !m_renderer.isStepPressed() &&
        !m_renderer.isClearPressed() && !m_renderer.isRandomPressed() && !m_renderer.isRulesPressed() &&
        !m_renderer.isColorAPressed() && !m_renderer.isColorBPressed()) {
        Vector2 cell = m_renderer.getMouseCell();
        int x = static_cast<int>(cell.x), y = static_cast<int>(cell.y);
        if (x >= 0 && x < m_grid.getWidth() && y >= 0 && y < m_grid.getHeight())
            m_grid.setCellColor(x, y, m_renderer.getDisplayColorA());
    }
    if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT)) {
        Vector2 cell = m_renderer.getMouseCell();
        int x = static_cast<int>(cell.x), y = static_cast<int>(cell.y);
        if (x >= 0 && x < m_grid.getWidth() && y >= 0 && y < m_grid.getHeight())
            m_grid.setCellColor(x, y, m_renderer.getDisplayColorB());
    }
}

void Game::updateSimulation() {
    m_grid.update();
}