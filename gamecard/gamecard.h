#ifndef GAMECARD_H
#define GAMECARD_H

#include "./general/infocard.h"
#include <QWidget>
#include "./gamecard/game.h"

class GameCard : public InfoCard
{
    Q_OBJECT
private:
    Game* game;
public:
    GameCard();
    ~GameCard();

public slots:
    void set_game(int game_id);

private slots:
    void on_changeButton_clicked();
    void cancel_changes();
    void save_changes();
    void delete_game();
    void on_backButton_clicked();

signals:
    void gameCard_backButton_clicked();

};

#endif // GAMECARD_H
