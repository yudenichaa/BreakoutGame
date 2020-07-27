#pragma once
#include "abstractblock.h"
#include <QColor>

class Brick : public AbstractBlock
{
public:
    Brick();
    void setColor(const QColor &color);
    void setBorderColor(const QColor &color);
};
