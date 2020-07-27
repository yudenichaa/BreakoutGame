#pragma once
#include <QObject>
#include <QGraphicsEllipseItem>
#include <QTimer>
#include <QVector2D>
#include "platform.h"
#include "brick.h"
#include "gameborder.h"

class Ball : public QObject, public QGraphicsEllipseItem {
    Q_OBJECT
public:
    Ball(const QRectF &gameArea);
    ~Ball();
    void startMoving();
    bool isBallMoving() const;
    qreal borderWidth() const;

private:
    QTimer *timer;
    QVector2D *direction;
    QPointF *prevPos;
    GameBorder *gameBorder;
    quint32 userLifesCount;
    quint32 score;
    bool ballStarted;

private:
    void rotate(qreal angleInRadians);
    // change the direction of the ball to the left by a given angle
    void rotateLeft(qreal angleInRadians);
    // change the direction of the ball to the right by a given angle
    void rotateRight(qreal angleInRadians);
    // calculation of the collision angle with respect to the x-axis
    qreal getCollisionAngleAboutX() const;
    // calculation of the collision angle with respect to the y-axis
    qreal getCollisionAngleAboutY() const;
    // handling the case when the object with which the collision occurred is to the left of the ball
    void collisionOnTheLeft();
    // handling the case when the object with which the collision occurred is to the right of the ball
    void collisionOnTheRight();
    // handling the case when the object with which the collision occurred is above of the ball
    void collisionOnTheTop();
    // handling the case when the object with which the collision occurred is below of the ball
    void collisionOnTheBottom();
    void collisionWithBrick(Brick *brick);
    void collisionWithPlatform(const Platform *platform);
    void checkForCollisions();
    void checkForContactWithTheBorder();
    // save current position defore changing the direction
    void updatePreviusPosition();
    bool isLevelComplete() const;
    void stopMoving();

signals:
    void signalBallReachedTheLowerBorder();
    // emited when user won or lose
    void signalGameEnds(bool isVictory, quint32 score);

private slots:
    // processes a timer timeout signal, moving the ball
    void move();
};
