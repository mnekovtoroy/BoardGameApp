#ifndef PLAYERCARD_H
#define PLAYERCARD_H

#include "./general/infocard.h"
#include <QWidget>
#include "./playercard/player.h"

class PlayerCard : public InfoCard
{
    Q_OBJECT
private:
    Player* player = nullptr;

public:
    PlayerCard();

public slots:
    void set_player(int player_id);
    void on_changeButton_clicked();
    void cancel_changes();
    void save_changes();
    void delete_player();
    void on_backButton_clicked();

signals:
    void playerCard_backButton_clicked();
};

#endif // PLAYERCARD_H
