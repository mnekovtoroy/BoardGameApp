#include "./playstab/playcard.h"
#include <QtSql>

PlayCard::PlayCard()
{
    name->setText("Партия"); //Since header is always the same

    //Adding change button
    QPushButton* changeButton = new QPushButton("Изменить",this);
    changeButton->setObjectName("chagneButton");
    headerLayout->addWidget(changeButton);
    //connect(changeButton,SIGNAL(clicked()),this,SLOT(on_changeButton_clicked()));

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
    query.prepare("SELECT play_game_expantion.play_id, game.name, play_game_expantion.expantion_id, play.play_date, play.description "
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
    game->addItem(query.value(rec.indexOf("name")).toString());
    game->setCurrentIndex(0);

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
    while (queryPlayers.next()) {
        QComboBox* player = new QComboBox();
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

        QHBoxLayout* playerLayout = new QHBoxLayout();
        playerLayout->addWidget(player);
        QSpacerItem* spacer = new QSpacerItem(100,1);
        playerLayout->addSpacerItem(spacer);
        //playerLayout->addStretch();
        playerLayout->addWidget(points);
        playerLayout->addStretch();

        players_list->addLayout(playerLayout);
    }
    winner->setText(winnerStr); //winner setup




}
