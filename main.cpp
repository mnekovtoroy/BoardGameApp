#include "mainwindow.h"

#include <QApplication>
#include <QLocale>
#include <QTranslator>
#include <QtSql>
#include <QSqlDatabase>
#include <QDebug>

//Database connect
static bool DBConnect() {
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("C:/Users/User/AppData/Roaming/BoardGamesTracker/BoardGamesTracker.db");
    if (!db.open()) {
        qDebug() << "Failed database connection: " << db.lastError();
        return false;
    }
    QSqlQuery query(db);
    if (!query.exec("PRAGMA foreign_keys = ON;")) {
        qDebug() << "Unable to turn on foreign_keys";
    }
    qDebug() << "sucsess";
    return true;
}

//create all tables in the database
bool DBCreate() {
    QSqlQuery query;
    QString queryStr;

    queryStr = "CREATE TABLE IF NOT EXISTS game("
               "    game_id INT PRIMARY KEY,"
               "    name VARCHAR(50) NOT NULL,"
               "    min_players INT NOT NULL,"
               "    max_players INT NOT NULL,"
               "    win_type VARCHAR(10) NOT NULL,"
               "    description VARCHAR(200)"
               ");"
            ;
    if (!query.exec(queryStr)) {
        qDebug() << "Unable to create a `game` table";
        return false;
    }

    queryStr = "CREATE TABLE IF NOT EXISTS player("
               "    player_id INT PRIMARY KEY,"
               "    name VARCHAR(50) NOT NULL,"
               "    description VARCHAR(200),"
               "    favorite_game_id INT,"
               "    FOREIGN KEY(favorite_game_id) REFERENCES game(game_id) ON DELETE SET NULL"
               ");"
            ;
    if (!query.exec(queryStr)) {
        qDebug() << "Unable to create a `player` table";
        return false;
    }

    queryStr = "CREATE TABLE IF NOT EXISTS play("
               "    play_id INT PRIMARY KEY,"
               "    play_date DATE NOT NULL,"
               "    description VARCHAR(200)"
               ");"
            ;
    if (!query.exec(queryStr)) {
        qDebug() << "Unable to create a `play` table";
        return false;
    }

    queryStr = "CREATE TABLE IF NOT EXISTS expantion("
               "    expantion_id INT PRIMARY KEY,"
               "    game_id INT,"
               "    name VARCHAR(50),"
               "    FOREIGN KEY(game_id) REFERENCES game(game_id) ON DELETE CASCADE"
               ");"
            ;
    if (!query.exec(queryStr)) {
        qDebug() << "Unable to create a `expantion` table";
        return false;
    }

    queryStr = "CREATE TABLE IF NOT EXISTS take_part("
               "    play_id INT,"
               "    player_id INT,"
               "    points INT NOT NULL,"
               "    PRIMARY KEY(play_id, player_id),"
               "    FOREIGN KEY(play_id) REFERENCES play(play_id) ON DELETE CASCADE,"
               "    FOREIGN KEY(player_id) REFERENCES player(player_id) ON DELETE NO ACTION"
               ");"
            ;
    if (!query.exec(queryStr)) {
        qDebug() << "Unable to create a `take_part` table";
        return false;
    }

    queryStr = "CREATE TABLE IF NOT EXISTS play_game_expantion("
               "    pge_relationship_id INT PRIMARY KEY,"
               "    play_id INT NOT NULL,"
               "    game_id INT NOT NULL,"
               "    expantion_id INT,"
               "    FOREIGN KEY(play_id) REFERENCES play(play_id) ON DELETE CASCADE,"
               "    FOREIGN KEY(game_id) REFERENCES game(game_id) ON DELETE NO ACTION,"
               "    FOREIGN KEY(expantion_id) REFERENCES expantion(expantion_id) ON DELETE NO ACTION"
               ");"
            ;
    if (!query.exec(queryStr)) {
        qDebug() << "Unable to create a `play_game_expantion` table";
        return false;
    }

    return true;
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QTranslator translator;
    const QStringList uiLanguages = QLocale::system().uiLanguages();
    for (const QString &locale : uiLanguages) {
        const QString baseName = "BoardGamesApp_" + QLocale(locale).name();
        if (translator.load(":/i18n/" + baseName)) {
            a.installTranslator(&translator);
            break;
        }
    }

    DBConnect();
    DBCreate();


    MainWindow window;
    window.show();
    return a.exec();
}
