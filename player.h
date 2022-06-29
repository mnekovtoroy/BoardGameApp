#ifndef PLAYER_H
#define PLAYER_H

#include<QApplication>

class Player
{
private:
    int player_id;
    QString name;
    QString description;
    int favorite_game_id;
    QString favorite_game_name;
    bool exists_in_DB = false;

    void set_from_DB(int player_id); //Set all fields from a database

public:
    Player();
    Player(int id);

    //Getters and setters
    void set_name(const QString &name);
    QString get_name();
    void set_description(const QString &description);
    QString get_description();
    void set_favorite_game(const int &id);
    int get_favorite_game();
    int get_player_id();
    QString get_fav_game_name();

    //DB interactions
    void update_DB();
};

#endif // PLAYER_H
