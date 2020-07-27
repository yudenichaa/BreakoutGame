#include "gameview.h"
#include "ball.h"
#include "brick.h"
#include <QKeyEvent>
#include <QMouseEvent>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QFile>
#include <QTextStream>
#include <QHash>
#include "gameresultwidget.h"

void GameView::keyPressEvent(QKeyEvent *event)
{
    if(ball->isBallMoving()) {
        qreal shift = scene->width() * 0.05;
        if(event->key() == Qt::Key_Left)
            platform->moveLeft(shift);
        else if(event->key() == Qt::Key_Right)
            platform->moveRight(shift);
    }
    else if(event->key() == Qt::Key_Space)
        ball->startMoving();
}

void GameView::mousePressEvent(QMouseEvent *event)
{
    if(event->button() == Qt::MouseButton::LeftButton) {
        if(ball->isBallMoving())
            platform->moveCenterToX(event->pos().x());
        else
            ball->startMoving();
    }
}

void GameView::mouseMoveEvent(QMouseEvent *event) {
    platform->moveCenterToX(event->pos().x());
}

void GameView::slotBallReachedTheLowerBorder() {
    moveBallAndPlatformToCenter();
    // remove one image of the user's life from the scene
    scene->removeItem(userLifes.back());
    userLifes.pop_back();
}

void GameView::slotGameEnds(bool isVictory, quint32 score) {
    QPointF centerPosition(this->pos().x() + this->rect().width() / 2,
                           this->pos().y() + this->rect().height() / 2);
    GameResultWidget *gameResults = new GameResultWidget(isVictory, score, highscore, centerPosition);
    QObject::connect(gameResults, &GameResultWidget::signalCloseWindow,
                     this, &GameView::slotCloseGameResults);
    gameResults->exec();
}

void GameView::slotCloseGameResults(quint32 score, bool levelComplete) {
    if(levelComplete && isNextLevelExists()) {
        ++levelID;
        moveBallAndPlatformToCenter();
        loadLevel(levelID);
    }
    else {
        emit signalGameEnds(score);
        this->close();
    }
}

void GameView::moveBallAndPlatformToCenter() {
    ball->setPos((scene->width() - ball->rect().width() - ball->borderWidth() * 2) / 2,
                 scene->height() - ball->rect().height() - platform->rect().height()
                 - (platform->borderWidth() + ball->borderWidth()) * 2);
    platform->setPos((scene->width() - platform->rect().width() - platform->borderWidth() * 2) / 2,
                     scene->height() - platform->rect().height() - platform->borderWidth() * 2);
}

bool GameView::isNextLevelExists() const {
    QSqlDatabase db = QSqlDatabase::database("breakout");
    db.setDatabaseName("breakout.db");
    db.open();
    QSqlQuery query(db);
    query.prepare("select id_level from level where id_level = :id_level;");
    query.bindValue(":id_level", QString::number(levelID + 1));
    query.exec();
    bool exists = query.next();
    db.close();
    return exists;
}

void GameView::loadLevel(quint32 levelNumber) {
    QSqlDatabase db = QSqlDatabase::database("breakout");
    db.setDatabaseName("breakout.db");
    db.open();
    QSqlQuery query(db);
    query.prepare("select width, height, x, y, color, border_color " \
                  "from brick " \
                  "where id_brick in " \
                  "( " \
                  "select id_brick " \
                  "from level_content " \
                  "where id_level = :id_level " \
                  ");"
                  );
    query.bindValue(":id_level", QString::number(levelNumber));
    query.exec();

    qreal width, height, x, y;
    QString color, borderColor;
    QSqlRecord record = query.record();
    while(query.next()) {
        width = query.value(record.indexOf("width")).toReal();
        height = query.value(record.indexOf("height")).toReal();
        x = query.value(record.indexOf("x")).toReal();
        y = query.value(record.indexOf("y")).toReal();
        color = query.value(record.indexOf("color")).toString();
        borderColor = query.value(record.indexOf("border_color")).toString();

        Brick *brick = new Brick();
        brick->setRect(0, 0, width, height);
        brick->setPos(x, y);
        brick->setColor(QColor(color));
        brick->setBorderColor(QColor(borderColor));

        scene->addItem(brick);
    }

    db.close();
}

GameView::GameView(quint32 levelNumber, quint32 Highscore) :
    QGraphicsView(), levelID(levelNumber), highscore(Highscore)
{
    this->setFixedSize(800, 600);
    this->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    this->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    // load config
    QHash<QString, qreal> config;
    QFile file("config");
    file.open(QIODevice::ReadOnly);
    QTextStream is(&file);
    while(!is.atEnd()) {
        QString line = is.readLine();
        if(line.isEmpty())
            continue;
        if(line.startsWith('#'))
            continue;
       int splitIndex = line.indexOf(':');
       QString key = line.left(splitIndex).trimmed();
       qreal value = line.right(line.length() - splitIndex - 1).trimmed().toDouble();
       config[std::move(key)] = value / 100;
    }

    // create graphics scene
    scene = new QGraphicsScene();
    scene->setSceneRect(0, 0, 800, 600);
    this->setScene(scene);

    // create platform
    platform = new Platform();
    platform->setRect(0, 0, scene->width() * config["platform_width"], scene->height() * config["platform_height"]);
    scene->addItem(platform);

    // create ball
    ball = new Ball(this->sceneRect());
    ball->setRect(0, 0, scene->width() * config["ball_diameter"], scene->width() * config["ball_diameter"]);
    scene->addItem(ball);

    QObject::connect(ball, &Ball::signalBallReachedTheLowerBorder,
                     this, &GameView::slotBallReachedTheLowerBorder);
    QObject::connect(ball, &Ball::signalGameEnds,
                     this, &GameView::slotGameEnds);

    moveBallAndPlatformToCenter();

    // create user lifes;
    for(int i = 0; i < 3; ++i) {
        QGraphicsEllipseItem *userLife = new QGraphicsEllipseItem();
        userLife->setRect(0, 0, scene->width() * config["user_life_diameter"], scene->width() * config["user_life_diameter"]);
        userLife->setPos(scene->width() - userLife->rect().width() * (i + 1) - userLife->pen().width(),
                         scene->height() - userLife->rect().height() - userLife->pen().width());
        userLife->setBrush(QBrush(Qt::green));
        userLifes.push_back(userLife);
        scene->addItem(userLife);
    }

    //load level
    loadLevel(levelNumber);
}

GameView::~GameView() {
    delete scene;
}

