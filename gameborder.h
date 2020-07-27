#pragma once
#include <QGraphicsLineItem>

class GameBorder
{
public:
    GameBorder(const QRectF &gameArea);
    ~GameBorder();
    const QGraphicsLineItem *getTopBorder() const;
    const QGraphicsLineItem *getBottomBorder() const;
    const QGraphicsLineItem *getLeftBorder() const;
    const QGraphicsLineItem *getRightBorder() const;

private:
    QGraphicsLineItem *topBorder;
    QGraphicsLineItem *bottomBorder;
    QGraphicsLineItem *leftBorder;
    QGraphicsLineItem *rightBorder;
};
