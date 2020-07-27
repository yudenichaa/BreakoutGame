#include "platform.h"
#include <QBrush>
#include <QGraphicsScene>
#include <QKeyEvent>

void Platform::moveLeft(qreal shift)
{
    if(this->pos().x() - shift > 0)
        this->setPos(this->pos().x() - shift, this->pos().y());
    else
        this->setPos(this->borderWidth(), this->pos().y());
}

void Platform::moveRight(qreal shift)
{
    if(this->pos().x() + this->rect().width() + shift < scene()->width())
        this->setPos(this->pos().x() + shift, this->pos().y());
    else
        this->setPos(scene()->width() - this->rect().width() - this->borderWidth() * 2, this->pos().y());
}

void Platform::moveCenterToX(qreal x)
{
    if(x > this->pos().x() + this->rect().width() / 2) {
        if(x < scene()->width() - this->rect().width() / 2)
            this->setPos(x - this->rect().width() / 2, this->pos().y());
        else
            this->setPos(scene()->width() - this->rect().width() - this->borderWidth() * 2, this->pos().y());
    }
    else {
        if(x > this->rect().width() / 2)
            this->setPos(x - this->rect().width() / 2, this->pos().y());
        else
            this->setPos(this->borderWidth(), this->pos().y());
    }
}

Platform::Platform() :
    AbstractBlock () {
    this->setBrush(QBrush(Qt::yellow));
}


































