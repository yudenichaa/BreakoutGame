#include "gameresultwidget.h"
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QVariant>

GameResultWidget::GameResultWidget(bool isVictory, quint32 score, quint32 bestScore, const QPointF &parentCenterPos) :
    QDialog(nullptr), currentScore(score), levelComplete(isVictory)
{
    this->setWindowFlag(Qt::WindowType::ToolTip);
    this->setFixedSize(250, 150);
    this->setFont(QFont("Cambria", 16));
    this->move(static_cast<int>(parentCenterPos.x()) - this->rect().width() / 2,
               static_cast<int>(parentCenterPos.y()) - this->rect().height() / 2);

    QLabel *lblIsVictory = new QLabel(isVictory ? "Level complete" : "You lose");
    lblIsVictory->setAlignment(Qt::AlignCenter);
    QLabel *lblCurrentScore = new QLabel("Current score: " + QString::number(currentScore));
    lblCurrentScore->setAlignment(Qt::AlignCenter);
    QLabel *lblBestScore = new QLabel("Best score: " +
                                      (currentScore > bestScore
                                       ? QString::number(currentScore)
                                       : QString::number(bestScore)));
    lblBestScore->setAlignment(Qt::AlignCenter);

    QPushButton *btnNext = new QPushButton(isVictory ? "Next level" : "Select level");
    QObject::connect(btnNext, &QPushButton::clicked,
                     this, &GameResultWidget::btnNextClicked);

    QVBoxLayout *gameResultsLauout = new QVBoxLayout();
    gameResultsLauout->setAlignment(Qt::AlignCenter);
    gameResultsLauout->addWidget(lblIsVictory);
    gameResultsLauout->addWidget(lblCurrentScore);
    gameResultsLauout->addWidget(lblBestScore);
    gameResultsLauout->addWidget(btnNext);

    this->setLayout(gameResultsLauout);
}

GameResultWidget::~GameResultWidget() {}

void GameResultWidget::btnNextClicked() {
    emit signalCloseWindow(currentScore, levelComplete);
    this->close();
}
