#include "./playercard/player.h"
#include <QtSql>
#include <stdexcept>
#include <QDebug>


Player::Player()
{
    name = "<New Player>";
    description = "No description";
    favorite_game_id = 1;

    QSqlQuery query;
    QString queryStr;
    queryStr = "SELECT player.player_id "
               "FROM player "
               "ORDER BY player_id DESC "
               "LIMIT 1;";
    if (!query.exec(queryStr)) {
        qDebug() << query.lastError();
        throw std::invalid_argument("Unable to select from `player` table");
    }

    query.next();
    player_id = query.value(0).toInt() + 1;
    exists_in_DB = false;
}

Player::Player(int id)
{
    try {
        set_from_DB(id);
    }  catch (const std::invalid_argument error) {
        throw std::invalid_argument(error.what());
    }
    exists_in_DB = true;
}

void Player::set_name(const QString &name) { this->name = name; }

QString Player::get_name() { return name; }

void Player::set_description(const QString &description) { this->description = description; }

QString Player::get_description() { return description; }

void Player::set_favorite_game(const int &id)
{
    this->favorite_game_id = id;

    QSqlQuery query;

    query.prepare("SELECT name "
                  "FROM game "
                  "WHERE game_id = :id; ");
    query.bindValue(":id", favorite_game_id);

    if (!query.exec()) {
        throw std::invalid_argument("Unable to select from `player` table");
    }

    QSqlRecord rec;

    rec = query.record();
    if (query.next()) {
        this->favorite_game_name = query.value(rec.indexOf("name")).toString();
    }
    else {
        throw std::invalid_argument("There is no such game id in the database");
    }
}

int Player::get_favorite_game() { return favorite_game_id; }

int Player::get_player_id() { return player_id; }

QString Player::get_fav_game_name() { return favorite_game_name; }

void Player::update_DB()
{
    if (exists_in_DB) {
        //if it is an existing game
        QSqlQuery query;
        query.prepare("UPDATE player "
                      "SET name = :name , description = :desc , favorite_game_id = :fav_game "
                      "WHERE player_id = :id; ");
        query.bindValue(":name",name);
        query.bindValue(":desc",description);
        query.bindValue(":fav_game",favorite_game_id);
        query.bindValue(":id",player_id);

        if (!query.exec()) {
            qDebug() << query.lastError();
            throw std::invalid_argument("Unable to update `player` table");
        }

    } else {
        //If it is a new player
        QSqlQuery query;
        query.prepare("INSERT INTO player "
                      "VALUES (:id, :name, :desc, :fav_game) ");
        query.bindValue(":name",name);
        query.bindValue(":desc",description);
        query.bindValue(":fav_game",favorite_game_id);
        query.bindValue(":id",player_id);
        if (!query.exec()) {
            qDebug() << query.lastError();
            throw std::invalid_argument("Unable to update `player` table");
        }
        exists_in_DB = true;
    }
}

void Player::set_from_DB(int player_id)
{
    QSqlQuery query;

    query.prepare("SELECT * "
                  "FROM player "
                  "WHERE player_id = :id; ");
    query.bindValue(":id", player_id);

    if (!query.exec()) {
        throw std::invalid_argument("Unable to select from `player` table");
    }


    QSqlRecord rec = query.record();

    if (query.next()) {
        this->name = query.value(rec.indexOf("name")).toString();
        this->description = query.value(rec.indexOf("description")).toString();
        this->favorite_game_id = query.value(rec.indexOf("favorite_game_id")).toInt();
        this->player_id = query.value(rec.indexOf("player_id")).toInt();
    }
    else {
        throw std::invalid_argument("There is no such game id in the database");
    }
    exists_in_DB = true;

    query.prepare("SELECT name "
                  "FROM game "
                  "WHERE game_id = :id; ");
    query.bindValue(":id", favorite_game_id);

    if (!query.exec()) {
        throw std::invalid_argument("Unable to select from `player` table");
    }

    rec = query.record();
    if (query.next()) {
        this->favorite_game_name = query.value(rec.indexOf("name")).toString();
    }
    else {
        throw std::invalid_argument("There is no such game id in the database");
    }
}
