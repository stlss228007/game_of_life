#include "Rules.h"
#include <algorithm>

Rules::Rules() {
    // Удалён цикл for (int i = 3; i < Cell::MAX_COLORS; ++i)
    // Теперь правила для цветов 3..9 не создаются автоматически
}

void Rules::addRule(const TransitionRule& rule) {
    m_rules[rule.fromColor].push_back(rule);
}

void Rules::clearRulesForColor(Cell::ColorIndex color) {
    m_rules[color].clear();
}

const std::vector<TransitionRule>& Rules::getRulesForColor(Cell::ColorIndex color) const {
    static const std::vector<TransitionRule> empty;
    auto it = m_rules.find(color);
    if (it != m_rules.end()) return it->second;
    return empty;
}