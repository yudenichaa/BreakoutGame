#pragma once
#include "abstractblock.h"

class Platform : public AbstractBlock
{
public:
    Platform();
    void moveLeft(qreal shift);
    void moveRight(qreal shift);
    void moveCenterToX(qreal x);
};
