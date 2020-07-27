#pragma once
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsEllipseItem>
#include "platform.h"
#include "ball.h"
#include <QStack>

class GameView : public QGraphicsView
{
    Q_OBJECT
public:
    GameView(quint32 levelNumber, quint32 Highscore);
    ~GameView() override;

protected:
    void keyPressEvent(QKeyEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;

private:
    void loadLevel(quint32 levelNumber);
    void moveBallAndPlatformToCenter();
    bool isNextLevelExists() const;

private:
    QGraphicsScene *scene;
    Platform *platform;
    Ball *ball;
    QStack<QGraphicsEllipseItem*> userLifes;
    quint32 levelID;
    quint32 highscore;

signals:
    // emited when user lose game or completed all levels
    void signalGameEnds(quint32 score);

private slots:
    void slotBallReachedTheLowerBorder();
    // emited when user lose game or completed current level
    void slotGameEnds(bool isVictory, quint32 score);
    // emited when window with game results closed
    void slotCloseGameResults(quint32 score, bool levelComplete);
};
