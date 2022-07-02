#include "playsminicard.h"
#include <QtSql>

void PlaysMiniCard::addDelimiter()
{
    QFrame* delimiter = new QFrame();
    delimiter->setMaximumWidth(1);
    delimiter->setMinimumWidth(1);
    delimiter->setStyleSheet("background-color: #D9D9D9;");
    layout->addWidget(delimiter);
}

void PlaysMiniCard::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        emit clicked();
    }
}

PlaysMiniCard::PlaysMiniCard(QWidget *parent, int play_id, showFlags flags)
    : QWidget{parent}
{
    this->play_id = play_id;
    this->setStyleSheet("background-color: #FAFAFA;");

    QSqlQuery queryDGD;
    queryDGD.prepare("SELECT play_game_expantion.play_id, game.name, play_game_expantion.expantion_id, play.play_date, play.description "
                     "FROM play_game_expantion "
                     "JOIN play "
                     "ON play.play_id = play_game_expantion.play_id "
                     "JOIN game "
                     "ON play_game_expantion.game_id = game.game_id "
                     "WHERE play.play_id = :id;");
    queryDGD.bindValue(":id",this->play_id);
    if (!queryDGD.exec()) {
        qDebug() << queryDGD.lastError();
        throw std::invalid_argument("Unable to select from `pge` table");
    }
    queryDGD.next();
    QSqlRecord recDGD = queryDGD.record();

    layout = new QHBoxLayout(this);

    if (flags & showFlags::DATES) {
        date = new QLabel(queryDGD.value(recDGD.indexOf("play_date")).toString());
        layout->addWidget(date);
        if (flags & (showFlags::DESCRIPTION | showFlags::WINNER | showFlags::PLAYERS | showFlags::GAME)) {
            addDelimiter();
        }
    }

    if (flags & showFlags::GAME) {
        game = new QLabel(queryDGD.value(recDGD.indexOf("name")).toString());
        layout->addWidget(game);
        if (flags & (showFlags::DESCRIPTION | showFlags::WINNER | showFlags::PLAYERS)) {
            addDelimiter();
        }
    }

    if (flags & (showFlags::PLAYERS | showFlags::WINNER)) {
        QSqlQuery queryPlayers;
        queryPlayers.prepare("SELECT player.name, take_part.points "
                             "FROM take_part "
                             "JOIN player "
                             "on take_part.player_id = player.player_id "
                             "WHERE take_part.play_id = :id;");
        queryPlayers.bindValue(":id",this->play_id);
        if (!queryPlayers.exec()) {
            qDebug() << queryDGD.lastError();
            throw std::invalid_argument("Unable to select from `pge` table");
        }
        QSqlRecord recPlayers = queryPlayers.record();

        int winner_points = -100000;
        QString winnerStr;

        players = new QVBoxLayout();
        while (queryPlayers.next()) {
            QLabel* player = new QLabel(queryPlayers.value(recPlayers.indexOf("name")).toString());
            players->addWidget(player);
            if (winner_points < queryPlayers.value(recPlayers.indexOf("points")).toInt()) {
                winner_points = queryPlayers.value(recPlayers.indexOf("points")).toInt();
                winnerStr = player->text();
            }
        }

        if (flags & showFlags::PLAYERS) {
            layout->addLayout(players);
            if (flags & (showFlags::DESCRIPTION | showFlags::WINNER)) {
                addDelimiter();
            }
        } else {
            delete layout;
            layout = nullptr;
        }

        if (flags & showFlags::WINNER) {
            winner = new QLabel(winnerStr);
            layout->addWidget(winner);
            if (flags & showFlags::DESCRIPTION) {
                addDelimiter();
            }
        }

    }

    if (flags & showFlags::DESCRIPTION) {
        description = new QLabel(queryDGD.value(recDGD.indexOf("description")).toString());
        layout->addWidget(description);
    }
}

int PlaysMiniCard::get_play_id() { return play_id; }
