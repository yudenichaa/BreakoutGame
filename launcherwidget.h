#pragma once
#include <QWidget>
#include <QLabel>

class LauncherWidget : public QWidget
{
    Q_OBJECT
public:
    LauncherWidget(QWidget *parent = nullptr);
    ~LauncherWidget() override;

private:
    QLabel *lblHighscore;
    quint32 highscore;

signals:

private slots:
    void btnLevelClicked();
    // emited when game window closed
    void slotGameEnds(quint32 score);
};
