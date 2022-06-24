#include "gamecard.h"
#include <QtWidgets>
#include "usefulfunctions.h"
#include <QtSql>

//constructor
GameCard::GameCard()
{
    connect(backButton,SIGNAL(clicked()),this,SLOT(on_backButton_clicked()));

    //Adding change button
    QPushButton* changeButton = new QPushButton("Изменить",this);
    changeButton->setObjectName("chagneButton");
    headerLayout->addWidget(changeButton);
    connect(changeButton,SIGNAL(clicked()),this,SLOT(on_changeButton_clicked()));


    //Num of players setup
    QHBoxLayout *num_of_pl_Layout = new QHBoxLayout();

    QLabel *num_of_players_label = new QLabel("Кол-во игроков: ");
    num_of_pl_Layout->addWidget(num_of_players_label);

    QLineEdit *min_players = new QLineEdit(this);
    min_players->setText(QString::number(1));
    min_players->setReadOnly(true);
    num_of_pl_Layout->addWidget(min_players);
    min_players->setObjectName("min_players");

    QLabel *deflabel = new QLabel("-");
    num_of_pl_Layout->addWidget(deflabel);

    QLineEdit *max_players = new QLineEdit();
    max_players->setText(QString::number(4));
    max_players->setReadOnly(true);
    num_of_pl_Layout->addWidget(max_players);
    max_players->setObjectName("max_players");

    rightColumn->addLayout(num_of_pl_Layout);


    //Wintype setup
    QHBoxLayout *wintype_Layout = new QHBoxLayout();

    QLabel *wintype_Label = new QLabel("Тип победы: ", this);
    wintype_Layout->addWidget(wintype_Label);

    QLabel *wintype = new QLabel("VP",this);
    wintype_Layout->addWidget(wintype);
    wintype->setObjectName("wintype");

    rightColumn->addLayout(wintype_Layout);


    //Description setup
    QTextEdit *description = new QTextEdit(this);
    description->setReadOnly(true);
    description->setObjectName("description");

    leftColumn->addWidget(description);


    //Spacer setups
    rightColumn->addStretch();
    leftColumn->addStretch();

}

GameCard::~GameCard()
{
    if (game != nullptr)  {
        delete game; // Clear game pointer
    }
}

void GameCard::set_game(int game_id = 0)
{
    if (game != nullptr)  {
        delete game; // Clear game pointer
    }
    if (game_id == 0) {
        game = new Game; //If "New Game" Button pressed
        game->update_DB();
    } else {
        game = new Game(game_id); //If already existing game opened
    }

    //Name and id
    name->setText(game->get_name());
    idLabel->setText(QString::number(game->get_game_id()));

    //Number of players
    this->findChild<QLineEdit*>("min_players")->setText(QString::number(game->get_min_players()));
    this->findChild<QLineEdit*>("max_players")->setText(QString::number(game->get_max_players()));

    //Wintype
    this->findChild<QLabel*>("wintype")->setText(game->get_win_type());

    //Decription
    this->findChild<QTextEdit*>("description")->setText(game->get_description());

    if (game_id == 0) {
        on_changeButton_clicked();
        delete this->findChild<QPushButton*>("cancel");
    }
}

void GameCard::on_changeButton_clicked()
{
    //set all stuff editable
    name->setReadOnly(false);
    this->findChild<QLineEdit*>("min_players")->setReadOnly(false);
    this->findChild<QLineEdit*>("max_players")->setReadOnly(false);
    this->findChild<QTextEdit*>("description")->setReadOnly(false);

    //delete change button
    delete this->findChild<QPushButton*>("chagneButton");


    //add all of control buttons
    QPushButton* delete_game = new QPushButton("Удалить", this);
    delete_game->setObjectName("delete");

    QPushButton* cancel = new QPushButton("Отменить", this);
    cancel->setObjectName("cancel");

    QPushButton* save_game = new QPushButton("Сохранить", this);
    save_game->setObjectName("save");

    headerLayout->addWidget(delete_game);
    headerLayout->addWidget(cancel);
    headerLayout->addWidget(save_game);

    connect(cancel,SIGNAL(clicked()),this,SLOT(cancel_changes()));
    connect(save_game,SIGNAL(clicked()),this,SLOT(save_changes()));
    connect(delete_game,SIGNAL(clicked()),this,SLOT(delete_game()));

    //hide back button
    backButton->hide();
}

void GameCard::cancel_changes()
{
    backButton->show(); // show back button
    clearLayout(headerLayout,3); // delete cancel, delete and save buttons

    //Adding back change button
    QPushButton* changeButton = new QPushButton("Изменить",this);
    changeButton->setObjectName("chagneButton");
    headerLayout->addWidget(changeButton);
    connect(changeButton,SIGNAL(clicked()),this,SLOT(on_changeButton_clicked()));

    //set all stuff uneditable
    name->setReadOnly(true);
    this->findChild<QLineEdit*>("min_players")->setReadOnly(true);
    this->findChild<QLineEdit*>("max_players")->setReadOnly(true);
    this->findChild<QTextEdit*>("description")->setReadOnly(true);

    set_game(game->get_game_id());
}

void GameCard::save_changes()
{
    game->set_name(name->text());
    game->set_description(this->findChild<QTextEdit*>("description")->toPlainText());
    game->set_min_players(this->findChild<QLineEdit*>("min_players")->text().toInt());
    game->set_max_players(this->findChild<QLineEdit*>("max_players")->text().toInt());
    //game->set_wintype
    game->update_DB();

    cancel_changes();
    //load_games_list();
}

void GameCard::delete_game()
{
    QSqlQuery query;
    query.prepare(" DELETE FROM game "
                  " WHERE game_id = (?); ");
    query.bindValue(":id", idLabel->text().toInt());
    query.addBindValue(idLabel->text().toInt());

    if (!query.exec()) {
        qDebug() << query.lastError();
        throw std::invalid_argument("Unable to delete from `game` table");
    }

    //load_games_list();

    backButton->show(); // show back button
    clearLayout(headerLayout,3); // delete cancel, delete and save buttons

    //set all stuff uneditable
    name->setReadOnly(true);
    this->findChild<QLineEdit*>("min_players")->setReadOnly(true);
    this->findChild<QLineEdit*>("max_players")->setReadOnly(true);
    this->findChild<QTextEdit*>("description")->setReadOnly(true);

    //Adding back change button
    QPushButton* changeButton = new QPushButton("Изменить",this);
    changeButton->setObjectName("chagneButton");
    headerLayout->addWidget(changeButton);
    connect(changeButton,SIGNAL(clicked()),this,SLOT(on_changeButton_clicked()));

    on_backButton_clicked();
}

void GameCard::on_backButton_clicked()
{
    emit gameCard_backButton_clicked();
}


