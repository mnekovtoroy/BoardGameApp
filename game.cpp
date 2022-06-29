#include "game.h"
#include <QtSql>
#include <stdexcept>


Game::Game()
{
    name = "<New Game>";
    description = "No description";
    min_players = 1;
    max_players = 4;
    win_type = "VP";

    QSqlQuery query;
    QString queryStr;
    queryStr = "SELECT game.game_id "
               "FROM game "
               "ORDER BY game_id DESC "
               "LIMIT 1;";
    if (!query.exec(queryStr)) {
        qDebug() << query.lastError();
        throw std::invalid_argument("Unable to select from `game` table");
    }

    query.next();
    game_id = query.value(0).toInt() + 1;
    exists_in_DB = false;
}

Game::Game(int DB_id) {
    try {
        set_from_DB(DB_id);
    }  catch (const std::invalid_argument error) {
        throw std::invalid_argument(error.what());
    }
    exists_in_DB = true;
}

void Game::set_name(const QString &name) { this->name = name; }

QString Game::get_name() { return name; }

void Game::set_description(const QString &description) { this->description = description; }

QString Game::get_description() { return description; }

void Game::set_min_players(const int &min_players) { this->min_players = min_players; }

int Game::get_min_players() { return min_players; }

void Game::set_max_players(const int &max_players) { this->max_players = max_players; }

int Game::get_max_players() { return max_players; }

QString Game::get_win_type() { return win_type; }

int Game::get_game_id() { return game_id; }

void Game::update_DB()
{
    if (exists_in_DB) {
        //if it is an existing game
        QSqlQuery query;
        query.prepare("UPDATE game "
                      "SET name = :name , description = :desc , min_players = :min_p , max_players = :max_p "
                      "WHERE game_id = :id; ");
        query.bindValue(":name",name);
        query.bindValue(":desc",description);
        query.bindValue(":min_p",min_players);
        query.bindValue(":max_p",max_players);
        query.bindValue(":id",game_id);

        if (!query.exec()) {
            throw std::invalid_argument("Unable to update `game` table");
        }

    } else {
        //If it is a new game
        QSqlQuery query;
        query.prepare("INSERT INTO game "
                      "VALUES (:id, :name, :min_p, :max_p, :wt, :desc)");
        query.bindValue(":name",name);
        query.bindValue(":desc",description);
        query.bindValue(":min_p",min_players);
        query.bindValue(":max_p",max_players);
        query.bindValue(":id",game_id);
        query.bindValue(":wt",win_type);
        if (!query.exec()) {
            throw std::invalid_argument("Unable to update `game` table");
        }
        exists_in_DB = true;

    }


}

void Game::set_from_DB(int game_id)
{
    QSqlQuery query;

    query.prepare("SELECT * "
                  "FROM game "
                  "WHERE game_id = :id; ");
    query.bindValue(":id", game_id);

    if (!query.exec()) {
        throw std::invalid_argument("Unable to select from `game` table");
    }

    QSqlRecord rec = query.record();

    if (query.next()) {
        this->name = query.value(rec.indexOf("name")).toString();
        this->description = query.value(rec.indexOf("description")).toString();
        this->win_type = query.value(rec.indexOf("win_type")).toString();
        this->game_id = query.value(rec.indexOf("game_id")).toInt();
        this->min_players = query.value(rec.indexOf("min_players")).toInt();
        this->max_players = query.value(rec.indexOf("max_players")).toInt();
    }
    else {
        throw std::invalid_argument("There is no such game id in the database");
    }
    exists_in_DB = true;
}

