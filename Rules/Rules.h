#pragma once
#include "../Cell/Cell.h"
#include <vector>
#include <unordered_map>

struct TransitionRule {
    Cell::ColorIndex fromColor;
    std::vector<int> neighborCounts;
    Cell::ColorIndex toColor;
    float probability;
};

class Rules {
public:
    Rules();
    
    void addRule(const TransitionRule& rule);
    void clearRulesForColor(Cell::ColorIndex color);
    const std::vector<TransitionRule>& getRulesForColor(Cell::ColorIndex color) const;
    
private:
    std::unordered_map<Cell::ColorIndex, std::vector<TransitionRule>> m_rules;
};