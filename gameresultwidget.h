#pragma once
#include <QDialog>

class GameResultWidget : public QDialog
{
    Q_OBJECT
public:
    GameResultWidget(bool isVictory, quint32 score, quint32 bestScore, const QPointF &position);
    ~GameResultWidget() override;

private:
    quint32 currentScore;
    bool levelComplete;

signals:
    // emited when current window is closing
    void signalCloseWindow(quint32 currentScore, bool levelComplete);

private slots:
    void btnNextClicked();
};
