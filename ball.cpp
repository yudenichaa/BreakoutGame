#include "ball.h"
#include "brick.h"
#include "platform.h"
#include <QGraphicsScene>
#include <QtMath>

Ball::Ball(const QRectF &gameArea) :
    QObject(nullptr), QGraphicsEllipseItem(),
    timer(new QTimer()), direction(new QVector2D(0, 0)),
    prevPos(new QPointF()), gameBorder(new GameBorder(gameArea)),
    userLifesCount(3), score(0), ballStarted(false)
{
    this->setBrush(QBrush(Qt::green));
    QObject::connect(timer, &QTimer::timeout,
                     this, &Ball::move);
}

Ball::~Ball() {
    delete timer;
    delete direction;
    delete prevPos;
    delete gameBorder;
}

qreal Ball::borderWidth() const {
    return this->pen().width();
}

void Ball::startMoving() {
    ballStarted = true;
    // set ball for moving to the top
    direction->setX(0);
    direction->setY(-1);
    timer->start(3);
}

void Ball::stopMoving() {
    this->timer->stop();
    ballStarted = false;
}

bool Ball::isBallMoving() const {
    return ballStarted;
}

void Ball::move() {
    checkForCollisions();
    if(isLevelComplete()) {
        stopMoving();
        emit signalGameEnds(true, score);
        return;
    }
    checkForContactWithTheBorder();
    updatePreviusPosition();
    moveBy(static_cast<qreal>(direction->x()),
           static_cast<qreal>(direction->y()));
}

void Ball::checkForCollisions() {
    for(QGraphicsItem *item : collidingItems()) {
        if(typeid(*item) == typeid(Brick))
            collisionWithBrick(dynamic_cast<Brick*>(item));
        else if(typeid(*item) == typeid(Platform))
            collisionWithPlatform(dynamic_cast<Platform*>(item));
    }
}

bool Ball::isLevelComplete() const {
    // Level completed if there are no bricks left
    return static_cast<quint32>(scene()->items().size()) - userLifesCount == 2;
}

void Ball::collisionWithBrick(Brick *brick) {
    QGraphicsLineItem topLine(brick->topLine());
    QGraphicsLineItem bottomLine(brick->bottomLine());
    QGraphicsLineItem leftLine(brick->leftLine());
    QGraphicsLineItem rightLine(brick->rightLine());
    if(this->collidesWithItem(&topLine))
        collisionOnTheBottom();
    else if(this->collidesWithItem(&bottomLine))
        collisionOnTheTop();
    else if (this->collidesWithItem(&leftLine))
        collisionOnTheRight();
    else
        collisionOnTheLeft();
    scene()->removeItem(brick);
    score += 1;
}

void Ball::collisionWithPlatform(const Platform *platform) {
    QGraphicsLineItem topLine(platform->topLine());
    QGraphicsLineItem bottomLine(platform->bottomLine());
    QGraphicsLineItem leftLine(platform->leftLine());
    QGraphicsLineItem rightLine(platform->rightLine());
    if(this->collidesWithItem(&topLine)) {
        // set ball for moving to the top
        direction->setX(0);
        direction->setY(-1);
        // calculating distance from collision point to platform center
        qreal ballCenter = this->pos().x() + this->rect().width() / 2;
        qreal platformCenter = platform->pos().x() + platform->rect().width() / 2;
        qreal distance = qAbs(ballCenter - platformCenter);
        // calculation rotation angle
        // 0 <= angle < 90
        // the angle of rotation depends on the distance of the collision point from the center of the platform
        // the greater the distance, the greater the angle
        qreal angle = (distance * 1.55) / platform->rect().width();
        if(ballCenter > platformCenter)
            rotateRight(angle);
        else
            rotateLeft(angle);
    }
    else if(this->collidesWithItem(&leftLine))
        collisionOnTheTop();
    else if (this->collidesWithItem(&rightLine))
        collisionOnTheRight();
    else
        collisionOnTheBottom();
}

void Ball::checkForContactWithTheBorder() {
    if(this->collidesWithItem(gameBorder->getTopBorder()))
        collisionOnTheTop();
    else if(this->collidesWithItem(gameBorder->getLeftBorder()))
        collisionOnTheLeft();
    else if(this->collidesWithItem(gameBorder->getRightBorder()))
        collisionOnTheRight();
    else if(this->collidesWithItem(gameBorder->getBottomBorder())) {
        stopMoving();
        emit signalBallReachedTheLowerBorder();
        if(--userLifesCount == 0)
            emit signalGameEnds(false, score);
    }
}

void Ball::collisionOnTheLeft() {
    qreal alpha = getCollisionAngleAboutX();
    if(prevPos->y() < this->pos().y())
        rotateLeft(2 * alpha);
    else
        rotateRight(2 * alpha);
}

void Ball::collisionOnTheRight() {
    qreal alpha = getCollisionAngleAboutX();
    if(prevPos->y() < this->pos().y())
        rotateRight(2 * alpha);
    else
        rotateLeft(2 * alpha);
}

void Ball::collisionOnTheTop() {
    qreal alpha = getCollisionAngleAboutY();
    if(prevPos->x() < this->pos().x())
        rotateRight(2 * alpha);
    else
        rotateLeft(2 * alpha);
}

void Ball::collisionOnTheBottom() {
    qreal alpha = getCollisionAngleAboutY();
    if(prevPos->x() < this->pos().x())
        rotateLeft(2 * alpha);
    else
        rotateRight(2 * alpha);
}

qreal Ball::getCollisionAngleAboutX() const {
    qreal a = qAbs(prevPos->x() - this->pos().x());
    qreal b = qAbs(prevPos->y() - this->pos().y());
    qreal c = qSqrt(a * a + b * b);
    return qAsin(a / c);
}

qreal Ball::getCollisionAngleAboutY() const {
    qreal a = qAbs(prevPos->y() - this->pos().y());
    qreal b = qAbs(prevPos->x() - this->pos().x());
    qreal c = qSqrt(a * a + b * b);
    return qAsin(a / c);
}

void Ball::rotateLeft(qreal angleInRadians) {
    rotate(-angleInRadians);
}

void Ball::rotateRight(qreal angleInRadians) {
    rotate(angleInRadians);
}

void Ball::rotate(qreal angleInRadians) { // pisitive angle - rotate right, negative angle - rotate left
    float x2 = direction->x() * static_cast<float>(qCos(angleInRadians))
            - direction->y() * static_cast<float>(qSin(angleInRadians));
    float y2 = direction->x() * static_cast<float>(qSin(angleInRadians))
            + direction->y() * static_cast<float>(qCos(angleInRadians));
    direction->setX(x2);
    direction->setY(y2);
}

void Ball::updatePreviusPosition() {
    prevPos->setX(this->pos().x());
    prevPos->setY(this->pos().y());
}
