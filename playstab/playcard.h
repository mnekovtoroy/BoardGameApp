#ifndef PLAYCARD_H
#define PLAYCARD_H

#include "./general/infocard.h"
#include <QWidget>
#include <QtWidgets>

class PlayCard : public InfoCard
{
    Q_OBJECT
public:
    PlayCard();

private:
    QHBoxLayout* game_layout;
    QComboBox* game;
    QHBoxLayout* date_layout;
    QLineEdit* date;
    QHBoxLayout* winner_layout;
    QLabel* winner;
    QTextEdit* description;
    QVBoxLayout* players;
    QVBoxLayout* players_list;

public slots:
    void set_play(int play_id);

private slots:
    void on_changeButton_clicked();
    void cancel_changes();
    void delete_player();
    void add_player();
};

#endif // PLAYCARD_H
