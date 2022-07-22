#include "./playstab/playcard.h"
#include "./general/usefulfunctions.h"
#include <QtSql>

static int playerLayoutId = 1;

PlayCard::PlayCard()
{
    name->setText("Партия"); //Since header is always the same
    connect(backButton,SIGNAL(clicked()),this,SLOT(on_backButton_clicked()));

    //Adding change button
    QPushButton* changeButton = new QPushButton("Изменить",this);
    changeButton->setObjectName("chagneButton");
    headerLayout->addWidget(changeButton);
    connect(changeButton,SIGNAL(clicked()),this,SLOT(on_changeButton_clicked()));

    //Game setup
    game_layout = new QHBoxLayout();
    game_layout->addWidget(new QLabel("Игра: "));
    game = new QComboBox();
    game->setEnabled(false);
    game_layout->addWidget(game);
    game_layout->addStretch();
    leftColumn->addLayout(game_layout);

    //Date setup
    date_layout = new QHBoxLayout();
    date_layout->addWidget(new QLabel("Дата: "));
    date = new QLineEdit();
    date->setFixedWidth(83);
    date->setReadOnly(true);
    date_layout->addWidget(date);
    date_layout->addStretch();
    leftColumn->addLayout(date_layout);

    //winner setup
    winner_layout = new QHBoxLayout();
    winner_layout->addWidget(new QLabel("Победитель: "));
    winner = new QLabel("winner");
    winner_layout->addWidget(winner);
    winner_layout->addStretch();
    leftColumn->addLayout(winner_layout);

    //description setup
    description = new QTextEdit();
    description->setMaximumWidth(300);
    description->setReadOnly(true);
    description->setSizePolicy(QSizePolicy::Minimum,QSizePolicy::Maximum);
    leftColumn->addWidget(description);

    //Players setup (empty by now)
    players = new QVBoxLayout();
    players->addWidget(new QLabel("Результаты партии:"));
    players_list = new QVBoxLayout();
    players->addLayout(players_list);
    leftColumn->addLayout(players);

    leftColumn->addStretch();
}

void PlayCard::set_play(int play_id)
{
    QSqlQuery query;
    query.prepare("SELECT play_game_expantion.play_id, game.game_id, game.name, play_game_expantion.expantion_id, play.play_date, play.description "
                  "FROM play_game_expantion "
                  "JOIN play "
                  "ON play.play_id = play_game_expantion.play_id "
                  "JOIN game "
                  "ON play_game_expantion.game_id = game.game_id "
                  "WHERE play.play_id = :id;");
    query.bindValue(":id", play_id);
    if (!query.exec()) {
        qDebug() << query.lastError();
        throw std::invalid_argument("Unable to select from `pge` table");
    }
    if (!query.next()) {
        throw std::invalid_argument("There is no such play");
    }
    QSqlRecord rec = query.record();

    //Id
    this->idLabel->setText(query.value(rec.indexOf("play_id")).toString());

    //Game
    //Fill game combobox
    QSqlQuery queryAllGames;
    queryAllGames.prepare("SELECT game.game_id, game.name "
                          "FROM game; ");
    queryAllGames.bindValue(":id", play_id);
    if (!queryAllGames.exec()) {
        qDebug() << queryAllGames.lastError();
        throw std::invalid_argument("Unable to select from `pge` table");
    }
    QSqlRecord recAllGames;
    recAllGames = queryAllGames.record();
    game->addItem("<Выберете игру>");

    while(queryAllGames.next()) {
        game->addItem(queryAllGames.value(recAllGames.indexOf("name")).toString() + ", " + queryAllGames.value(recAllGames.indexOf("game_id")).toString());
    }
    //Set current game
    game->setCurrentText(query.value(rec.indexOf("name")).toString() + ", " + query.value(rec.indexOf("game_id")).toString());

    //Date
    date->setText(query.value(rec.indexOf("play_date")).toString());

    //Description
    description->setPlainText(query.value(rec.indexOf("description")).toString());

    //Players
    QSqlQuery queryPlayers;
    queryPlayers.prepare("SELECT player.player_id, player.name, take_part.points "
                         "FROM take_part "
                         "JOIN player "
                         "on take_part.player_id = player.player_id "
                         "WHERE take_part.play_id = :id;");
    queryPlayers.bindValue(":id", play_id);
    if (!queryPlayers.exec()) {
        qDebug() << queryPlayers.lastError();
        throw std::invalid_argument("Unable to select from `pge` table");
    }
    QSqlRecord recPlayers = queryPlayers.record();

    int winner_points = -100000;
    QString winnerStr;


    //Adding players layouts, one by one
    playerLayoutId = 1;
    while (queryPlayers.next()) {
        QComboBox* player = new QComboBox();
        player->setObjectName(QString::number(playerLayoutId));
        player->setEnabled(false);

        //Fill player combobox
        QSqlQuery queryAllPl;
        queryAllPl.prepare("SELECT player_id, name "
                           "FROM player; ");

        if (!queryAllPl.exec()) {
            throw std::invalid_argument("Unable to select from `player` table");
        }
        QSqlRecord recAllPl;
        recAllPl = queryAllPl.record();
        player->addItem("<Выберете игрока>");

        while(queryAllPl.next()) {
            player->addItem(queryAllPl.value(recAllPl.indexOf("name")).toString() + ", " + queryAllPl.value(recAllPl.indexOf("player_id")).toString());
        }

        //Set current player
        player->setCurrentText(queryPlayers.value(recPlayers.indexOf("name")).toString() + ", " + queryPlayers.value(recPlayers.indexOf("player_id")).toString());

        if (winner_points < queryPlayers.value(recPlayers.indexOf("points")).toInt()) { //Winner prep
            winner_points = queryPlayers.value(recPlayers.indexOf("points")).toInt();
            winnerStr = player->currentText();
        }

        //Points
        QLineEdit* points = new QLineEdit();
        points->setPlaceholderText("...");
        points->setText(queryPlayers.value(recPlayers.indexOf("points")).toString());
        points->setMaximumWidth(28);
        points->setReadOnly(true);
        points->setAlignment(Qt::AlignHCenter);
        points->setObjectName(QString::number(playerLayoutId));

        QHBoxLayout* playerLayout = new QHBoxLayout();
        playerLayout->addWidget(player);
        QSpacerItem* spacer = new QSpacerItem(100,1);
        playerLayout->addSpacerItem(spacer);
        playerLayout->addWidget(points);
        playerLayout->setObjectName(QString::number(playerLayoutId));
        playerLayout->addStretch();

        players_list->addLayout(playerLayout);
        playerLayoutId++;
    }
    winner->setText(winnerStr); //winner setup




}

void PlayCard::on_changeButton_clicked()
{
    //Set all stuff editable
    //Description
    description->setReadOnly(false);

    //Game
    game->setEnabled(true);

    //Date
    date->setReadOnly(false);

    //Players
    //players_list
    for (int i = 0; i < players_list->count(); i++) {
        QPushButton* deletePlayer = new QPushButton("X");
        deletePlayer->setMaximumWidth(20);
        deletePlayer->setObjectName(QString::number(i+1));
        this->findChild<QHBoxLayout*>(QString::number(i+1))->insertWidget(3,deletePlayer);
        //players_list->itemAt(i)->layout()->addWidget(deletePlayer);
        connect(deletePlayer,SIGNAL(clicked()),this,SLOT(delete_player()));

        this->findChild<QComboBox*>(QString::number(i+1))->setEnabled(true); //Player combobox
        this->findChild<QLineEdit*>(QString::number(i+1))->setReadOnly(false); //Points

    }
    QHBoxLayout* newPlLayout = new QHBoxLayout();
    QPushButton* addPlayer = new QPushButton("Добавить игрока");
    newPlLayout->addWidget(addPlayer);
    newPlLayout->addStretch();
    connect(addPlayer,SIGNAL(clicked()),this,SLOT(add_player()));
    players->addLayout(newPlLayout);

    //delete change button
    delete this->findChild<QPushButton*>("chagneButton");


    //add all of control buttons
    QPushButton* delete_play = new QPushButton("Удалить", this);
    delete_play->setObjectName("delete");

    QPushButton* cancel = new QPushButton("Отменить", this);
    cancel->setObjectName("cancel");

    QPushButton* save_play = new QPushButton("Сохранить", this);
    save_play->setObjectName("save");

    headerLayout->addWidget(delete_play);
    headerLayout->addWidget(cancel);
    headerLayout->addWidget(save_play);

    connect(cancel,SIGNAL(clicked()),this,SLOT(cancel_changes()));
    connect(save_play,SIGNAL(clicked()),this,SLOT(save_changes()));
    connect(delete_play,SIGNAL(clicked()),this,SLOT(delete_play()));

    //hide back button
    backButton->hide();
}

void PlayCard::cancel_changes()
{
    backButton->show(); // show back button
    clearLayout(headerLayout,3); // delete cancel, delete and save buttons

    //Adding back change button
    QPushButton* changeButton = new QPushButton("Изменить",this);
    changeButton->setObjectName("chagneButton");
    headerLayout->addWidget(changeButton);
    connect(changeButton,SIGNAL(clicked()),this,SLOT(on_changeButton_clicked()));

    //set all stuff uneditable
    description->setReadOnly(true); //Description
    game->setEnabled(false); //Game
    date->setReadOnly(true); //Date
    clearLayout(players_list); //Players - clering layout cause it will load again
    clearLayout(players,2); //clear new player button


    this->set_play(this->idLabel->text().toInt());
}

void PlayCard::save_changes()
{
    QSqlQuery query;
    query.prepare("UPDATE play "
                  "SET play_date = :date, description = :desc "
                  "WHERE play_id = :id;");
    query.bindValue(":date",date->text());
    query.bindValue(":desc", description->toPlainText());
    query.bindValue(":id", idLabel->text());
    if (!query.exec()) {
        qDebug() << query.lastError();
        throw std::invalid_argument("Unable to update `play` table");
    }

    query.prepare("UPDATE play_game_expantion "
                  "SET game_id = :game_id "
                  "WHERE play_id = :play_id;");
    query.bindValue(":game_id", get_last_number(game->currentText()));
    query.bindValue(":play_id", idLabel->text());
    if (!query.exec()) {
        qDebug() << query.lastError();
        throw std::invalid_argument("Unable to update `pge` table");
    }

    query.prepare("DELETE FROM take_part "
                  "WHERE play_id = :id;");
    query.bindValue(":id", idLabel->text());
    if (!query.exec()) {
        qDebug() << query.lastError();
        throw std::invalid_argument("Unable to delete from `take_part` table");
    }

    for(int i = 0; i < players_list->count(); i++) {
        query.prepare("INSERT INTO take_part "
                      "VALUES (:play_id, :player_id, :points)");
        QString currPlayerLayoutId = players_list->itemAt(i)->layout()->objectName();
        query.bindValue(":play_id", idLabel->text());
        query.bindValue(":player_id", get_last_number(this->findChild<QComboBox*>(currPlayerLayoutId)->currentText()));
        query.bindValue(":points",this->findChild<QLineEdit*>(currPlayerLayoutId)->text().toInt());
        if (!query.exec()) {
            qDebug() << query.lastError();
            throw std::invalid_argument("Unable to insert into `take_part` table");
        }
    }

    cancel_changes();

}

void PlayCard::delete_play()
{
    QSqlQuery query;
    query.prepare("DELETE FROM play "
                  "WHERE play_id = :id;");
    query.bindValue(":id", idLabel->text());
    if (!query.exec()) {
        qDebug() << query.lastError();
        throw std::invalid_argument("Unable to delete from `play` table");
    }

    query.prepare("DELETE FROM play_game_expantion "
                  "WHERE play_id = :id;");
    query.bindValue(":id", idLabel->text());
    if (!query.exec()) {
        qDebug() << query.lastError();
        throw std::invalid_argument("Unable to delete from `play_game_expantion` table");
    }

    query.prepare("DELETE FROM take_part "
                  "WHERE play_id = :id;");
    query.bindValue(":id", idLabel->text());
    if (!query.exec()) {
        qDebug() << query.lastError();
        throw std::invalid_argument("Unable to delete from `take_part` table");
    }

    on_backButton_clicked();

}

void PlayCard::delete_player()
{
    int id = sender()->objectName().toInt(); //player layout id stored in the sender button name
    QHBoxLayout* player = this->findChild<QHBoxLayout*>(QString::number(id));
    clearLayout(player);
    delete player;
}

void PlayCard::add_player()
{
    QHBoxLayout* playerLayout = new QHBoxLayout();
    playerLayout->setObjectName(QString::number(playerLayoutId)); //generating unice id for this layout so it can be easily deleted

    QComboBox* player = new QComboBox();
    player->setObjectName(QString::number(playerLayoutId));
    playerLayout->addWidget(player);
    playerLayout->addSpacerItem(new QSpacerItem(100,1));

    QLineEdit* points = new QLineEdit();
    points->setPlaceholderText("...");
    points->setMaximumWidth(28);
    points->setAlignment(Qt::AlignHCenter);
    points->setObjectName(QString::number(playerLayoutId));
    playerLayout->addWidget(points);


    QPushButton* deletePlayer = new QPushButton("X");
    deletePlayer->setMaximumWidth(20);
    deletePlayer->setObjectName(QString::number(playerLayoutId)); //layout id will be stored in this button name so it will be reachable
    playerLayout->addWidget(deletePlayer);
    playerLayout->addStretch();

    connect(deletePlayer,SIGNAL(clicked()),this,SLOT(delete_player()));


    //Fill player combobox
    QSqlQuery query;
    query.prepare("SELECT player_id, name "
                  "FROM player; ");

    if (!query.exec()) {
        throw std::invalid_argument("Unable to select from `player` table");
    }

    QSqlRecord rec;
    rec = query.record();
    player->addItem("<Выберете игрока>");
    while(query.next()) {
        player->addItem(query.value(rec.indexOf("name")).toString() + ", " + query.value(rec.indexOf("player_id")).toString());
    }

    players_list->addLayout(playerLayout);

    playerLayoutId++;
}

void PlayCard::on_backButton_clicked()
{
    emit playCard_backButton_clicked();
}
