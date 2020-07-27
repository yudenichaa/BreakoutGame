#include "abstractblock.h"
#include <QPen>

AbstractBlock::AbstractBlock() {}

qreal AbstractBlock::borderWidth() const {
    return this->pen().width();
}

QLineF AbstractBlock::topLine() const {
    return QLineF(left(), top(), right(), top());
}

QLineF AbstractBlock::bottomLine() const {
    return QLineF(left(), bottom(), right(), bottom());
}

QLineF AbstractBlock::leftLine() const {
    return QLineF(left(), top(), left(), bottom());
}

QLineF AbstractBlock::rightLine() const {
    return QLineF(right(), top(), right(), bottom());
}

qreal AbstractBlock::left() const {
    return this->pos().x();
}

qreal AbstractBlock::right() const {
    return this->pos().x() + this->rect().width();
}

qreal AbstractBlock::top() const {
    return this->pos().y();
}

qreal AbstractBlock::bottom() const {
    return this->pos().y() + this->rect().height();
}
