#include "Cell.h"

Cell::Cell(ColorIndex color) : m_color(color) {}

Cell::ColorIndex Cell::getColor() const {
    return m_color;
}

void Cell::setColor(ColorIndex color) {
    if (color >= 1 && color < MAX_COLORS) m_color = color;
}