#include "gameborder.h"

GameBorder::GameBorder(const QRectF &gameArea) {
    topBorder = new QGraphicsLineItem(gameArea.left(), gameArea.top(),
                                     gameArea.right(), gameArea.top());
    bottomBorder = new QGraphicsLineItem(gameArea.left(), gameArea.bottom(),
                                         gameArea.right(), gameArea.bottom());
    leftBorder = new QGraphicsLineItem(gameArea.left(), gameArea.top(),
                                       gameArea.left(), gameArea.bottom());
    rightBorder = new QGraphicsLineItem(gameArea.right(), gameArea.top(),
                                        gameArea.right(), gameArea.bottom());
}

GameBorder::~GameBorder() {
    delete topBorder;
    delete bottomBorder;
    delete leftBorder;
    delete rightBorder;
}

const QGraphicsLineItem *GameBorder::getTopBorder() const {
    return topBorder;
}

const QGraphicsLineItem *GameBorder::getBottomBorder() const {
    return bottomBorder;
}

const QGraphicsLineItem *GameBorder::getLeftBorder() const {
    return leftBorder;
}

const QGraphicsLineItem *GameBorder::getRightBorder() const {
    return rightBorder;
}
