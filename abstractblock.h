#pragma once
#include <QGraphicsRectItem>
#include <QLineF>

class AbstractBlock : public QGraphicsRectItem
{
public:
    qreal borderWidth() const;
    QLineF topLine() const;
    QLineF bottomLine() const;
    QLineF leftLine() const;
    QLineF rightLine() const;

private:
    qreal left() const;
    qreal right() const;
    qreal top() const;
    qreal bottom() const;

protected:
    AbstractBlock();
};
