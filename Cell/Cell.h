#pragma once
#include <cstdint>

class Cell {
public:
    using ColorIndex = uint8_t;
    static constexpr ColorIndex MAX_COLORS = 10;
    
    Cell(ColorIndex color = 1);
    ColorIndex getColor() const;
    void setColor(ColorIndex color);
private:
    ColorIndex m_color;
};