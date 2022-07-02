#ifndef PLAYSMINICARD_H
#define PLAYSMINICARD_H

#include <QWidget>
#include <QtWidgets>


enum showFlags {
    DATES        = 1 << 0,
    GAME        = 1 << 1,
    PLAYERS     = 1 << 2,
    WINNER      = 1 << 3,
    DESCRIPTION = 1 << 4
};

class PlaysMiniCard : public QWidget
{
    Q_OBJECT
private:
    QHBoxLayout* layout = nullptr;
    QLabel* date = nullptr;
    QLabel* game = nullptr;
    QVBoxLayout* players = nullptr;
    QLabel* winner = nullptr;
    QLabel* description = nullptr;

    int play_id;

    void addDelimiter();

protected:
    virtual void mousePressEvent(QMouseEvent *event) override;

public:

    explicit PlaysMiniCard(QWidget *parent = nullptr, int play_id = 0, showFlags flags = (showFlags)(DATES | GAME | PLAYERS | WINNER | DESCRIPTION));

    int get_play_id();

signals:
    void clicked();
};

#endif // PLAYSMINICARD_H
