#include "brick.h"
#include <QBrush>
#include <QPen>

Brick::Brick() :
    AbstractBlock() {
}

void Brick::setColor(const QColor &color) {
    this->setBrush(color);
}

void Brick::setBorderColor(const QColor &color) {
    this->setPen(color);
}
