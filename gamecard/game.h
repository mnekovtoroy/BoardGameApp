#ifndef GAME_H
#define GAME_H

#include<QApplication>

class Game
{
private:
    QString name;
    QString description;
    int min_players;
    int max_players;
    QString win_type;
    int game_id;
    bool exists_in_DB = false;

    void set_from_DB(int game_id); //Set all fields from a database

public:
    Game(); //Default constructor;
    Game(int DB_id); //Constructor for the game already presented in the DB

    //Getters and setters
    void set_name(const QString &name);
    QString get_name();
    void set_description(const QString &description);
    QString get_description();
    void set_min_players(const int &min_players);
    int get_min_players();
    void set_max_players(const int &max_players);
    int get_max_players();
    QString get_win_type();
    int get_game_id();

    //DB interactions
    void update_DB();
};


#endif // GAME_H
