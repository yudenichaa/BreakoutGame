#include "launcherwidget.h"
#include "gameview.h"
#include <QPushButton>
#include <QBoxLayout>
#include <QVariant>
#include <QFile>
#include <QDataStream>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlRecord>

#include <QDebug>
void LauncherWidget::btnLevelClicked() {
    quint32 levelNumber = this->sender()->property("tag").toUInt();
    GameView *gameView = new GameView(levelNumber, highscore);
    QObject::connect(gameView, &GameView::signalGameEnds,
                     this, &LauncherWidget::slotGameEnds);
    this->hide();
    gameView->show();
}

void LauncherWidget::slotGameEnds(quint32 score) {
    if(score > highscore) {
        lblHighscore->setText("Highscore: " + QString::number(score));
        // save new highscore to the file highscore.dat
        QFile file("highscore.dat");
        file.open(QIODevice::WriteOnly);
        QDataStream out(&file);
        out << score;
        file.close();
    }
    this->show();
}

LauncherWidget::LauncherWidget(QWidget *parent)
    : QWidget(parent) {
    this->setWindowFlag(Qt::MSWindowsFixedSizeDialogHint);
    this->setFont(QFont("Cambria", 16));

    //read highscore
    QFile file("highscore.dat");
    file.open(QIODevice::ReadOnly);
    QDataStream in(&file);
    in >> highscore;
    file.close();

    lblHighscore = new QLabel("Highscore: " + QString::number(highscore));
    lblHighscore->setAlignment(Qt::AlignHCenter);

    // load levels
    QSqlDatabase db = QSqlDatabase::database("breakout");
    db.setDatabaseName("breakout.db");
    db.open();
    QSqlQuery query(db);
    query.exec("select name from level;");

    QVBoxLayout *levelsLayout = new QVBoxLayout();
    int row = 0;
    int levelsInRow = 3;
    do {
        QHBoxLayout *rowWithLevels = new QHBoxLayout;
        for(int i = 0; i < levelsInRow && query.next(); ++i) {
            QPushButton *btnLevel = new QPushButton(query.value(0).toString());
            QObject::connect(btnLevel, &QPushButton::clicked,
                             this, &LauncherWidget::btnLevelClicked);
            btnLevel->setProperty("tag", QVariant(row * levelsInRow + i));
            rowWithLevels->addWidget(btnLevel);
        }
        levelsLayout->addLayout(rowWithLevels);
        ++row;
    } while(query.isValid());

    db.close();

    //set layout
    QVBoxLayout *launcherLayout = new QVBoxLayout();
    launcherLayout->addWidget(lblHighscore);
    launcherLayout->addLayout(levelsLayout);
    this->setLayout(launcherLayout);
}

LauncherWidget::~LauncherWidget() {}
