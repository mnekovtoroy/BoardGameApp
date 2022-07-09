#include "./playstab/newplaydialog.h"
#include "./general/usefulfunctions.h"
#include <QtSql>

static int playerLayoutId = 0;

NewPlayDialog::NewPlayDialog()
{

    layout = new QVBoxLayout(this);

    //header setup
    game_date_layout = new QHBoxLayout();
    gameLabel = new QLabel("Игра: ");
    game_date_layout->addWidget(gameLabel);

        //game combobox
    game = new QComboBox();
    game->setMinimumWidth(120);
    game_date_layout->addWidget(game);
    connect(game,SIGNAL(currentIndexChanged(int)),this,SLOT(game_selected(int)));

    dateLabel = new QLabel("Дата: ");
    game_date_layout->addSpacerItem(new QSpacerItem(70,1,QSizePolicy::Minimum,QSizePolicy::Fixed));
    game_date_layout->addWidget(dateLabel);

    date = new QLineEdit();
    date->setMinimumWidth(83);
    date->setPlaceholderText("YYYY-MM-DD");
    game_date_layout->addWidget(date);

    layout->addLayout(game_date_layout);

    //players layout, its empty by now
    players = new QVBoxLayout();
    players->setObjectName("players_layout");
    layout->addLayout(players);


    //Buttons setup
    buttonsLayout = new QHBoxLayout();
    buttonsLayout->addStretch();
    OK_button = new QPushButton("Готово");
    cancel_button = new QPushButton("Отмена");
    buttonsLayout->addWidget(cancel_button);
    buttonsLayout->addWidget(OK_button);
    connect(cancel_button,SIGNAL(clicked()),this,SLOT(reject()));
    connect(OK_button,SIGNAL(clicked()),this,SLOT(OK_button_clicked()));

    layout->addStretch();
    layout->addLayout(buttonsLayout);

    //game ComboBox fill
    QSqlQuery query;
    query.prepare("SELECT game_id, name "
                  "FROM game; ");

    if (!query.exec()) {
        throw std::invalid_argument("Unable to select from `player` table");
    }

    QSqlRecord rec;
    rec = query.record();
    game->addItem("<Выбрать игру>");
    while(query.next()) {
        game->addItem(query.value(rec.indexOf("name")).toString() + ", " + query.value(rec.indexOf("game_id")).toString());
    }



}

void NewPlayDialog::game_selected(int index)
{
    clearLayout(players);
    if(index == 0) {
        return; //If no game was selected
    }
    //header setup
    QHBoxLayout* header = new QHBoxLayout();
    header->addWidget(new QLabel("Игрок"));
    header->addStretch();
    header->addWidget(new QLabel("Результат"));
    header->setObjectName("-1");
    players->addLayout(header);

    QVBoxLayout* playersList = new QVBoxLayout();
    playersList->setObjectName("players_list");
    players->addLayout(playersList);

    addPlayer(); //1st player

    QHBoxLayout* newPlLayout = new QHBoxLayout();
    QPushButton* addPlayer = new QPushButton("Добавить игрока");
    newPlLayout->addWidget(addPlayer);
    newPlLayout->addStretch();
    newPlLayout->setObjectName("-1");
    players->addLayout(newPlLayout);

    connect(addPlayer,SIGNAL(clicked()),this,SLOT(addPlayer()));
}

void NewPlayDialog::addPlayer()
{
    QHBoxLayout* playerLayout = new QHBoxLayout();
    playerLayout->setObjectName(QString::number(playerLayoutId)); //generating unice id for this layout so it can be easily deleted

    QComboBox* player = new QComboBox();
    player->setObjectName(QString::number(playerLayoutId));
    playerLayout->addWidget(player);
    playerLayout->addStretch();

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

    connect(deletePlayer,SIGNAL(clicked()),this,SLOT(deletePlayer()));


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

    this->findChild<QVBoxLayout*>("players_list")->addLayout(playerLayout);

    playerLayoutId++;
}

void NewPlayDialog::deletePlayer()
{
    int id = sender()->objectName().toInt(); //player layout id stored in the sender button name
    QHBoxLayout* player = this->findChild<QHBoxLayout*>(QString::number(id));
    clearLayout(player);
    delete player;
}

void NewPlayDialog::OK_button_clicked()
{
    QSqlQuery query;

    //get last used play_id
    query.prepare("SELECT play_id "
                  "FROM play "
                  "ORDER BY play_id DESC "
                  "LIMIT 1;");
    if (!query.exec()) {
        throw std::invalid_argument("Unable to select from `play` table");
    }
    query.next();
    int play_id = query.value(0).toInt() + 1; //avialable play_id

    //insert int play table
    query.prepare("INSERT INTO play "
                  "VALUES (:id, :date, '...');");
    query.bindValue(":id",play_id);
    query.bindValue(":date",this->date->text());
    if (!query.exec()) {
        throw std::invalid_argument("Unable to insert into `play` table");
    }

    //get last used pge_relationship_id
    query.prepare("SELECT pge_relationship_id "
                  "FROM play_game_expantion "
                  "ORDER BY pge_relationship_id DESC "
                  "LIMIT 1;");
    if (!query.exec()) {
        throw std::invalid_argument("Unable to select from `play_game_expantion` table");
    }
    query.next();
    int pge_id = query.value(0).toInt() + 1; //avialable pge_relationship_id

    //insert int play_game_expantion table
    query.prepare("INSERT INTO play_game_expantion "
                  "VALUES (:pge_id, :play_id, :game_id, NULL);");
    query.bindValue(":pge_id",pge_id);
    query.bindValue(":play_id",play_id);
    query.bindValue(":game_id",get_last_number(this->game->currentText()));
    if (!query.exec()) {
        throw std::invalid_argument("Una5ble to insert into `play_game_expantion` table");
    }

    //insert players
    int current_player = 0;
    while(current_player < players->itemAt(1)->layout()->count()) {
        query.prepare("INSERT INTO take_part "
                      "VALUES (:play_id, :player_id, :points);");
        query.bindValue(":play_id",play_id);
        query.bindValue(":player_id",get_last_number(this->findChild<QComboBox*>(players->itemAt(1)->layout()->itemAt(current_player)->layout()->objectName())->currentText()));
        query.bindValue(":points",this->findChild<QLineEdit*>(players->itemAt(1)->layout()->itemAt(current_player)->layout()->objectName())->text().toInt());
        if (!query.exec()) {
            throw std::invalid_argument("Unable to insert into `take_part` table");
        }
        current_player++;
    }
    accept();
}
