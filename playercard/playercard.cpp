#include "./playercard/playercard.h"
#include <QtSql>
#include "./general/usefulfunctions.h"

PlayerCard::PlayerCard()
{
    connect(backButton,SIGNAL(clicked()),this,SLOT(on_backButton_clicked()));

    //Adding change button
    QPushButton* changeButton = new QPushButton("Изменить",this);
    changeButton->setObjectName("chagneButton");
    headerLayout->addWidget(changeButton);
    connect(changeButton,SIGNAL(clicked()),this,SLOT(on_changeButton_clicked()));

    //Description setup
    QTextEdit *description = new QTextEdit(this);
    description->setReadOnly(true);
    description->setObjectName("description");

    leftColumn->addWidget(description);

    //Favorite game setup
    QHBoxLayout *fav_game_layout = new QHBoxLayout();
    QLabel *fav_game_label = new QLabel("Любимая игра: ",this);
    fav_game_layout->addWidget(fav_game_label);

    QComboBox *fav_game = new QComboBox(this);
    fav_game->setDisabled(true);
    fav_game->setObjectName("favorite_game");
    fav_game->setMinimumWidth(100);
    fav_game_layout->addWidget(fav_game);

    rightColumn->addLayout(fav_game_layout);

    //Spacer setups
    rightColumn->addStretch();
    leftColumn->addStretch();
}

void PlayerCard::set_player(int player_id)
{
    if (player != nullptr)  {
        delete player; // Clear game pointer
    }
    if (player_id == 0) {
        player = new Player; //If "New Game" Button pressed
        player->update_DB();
    } else {
        player = new Player(player_id); //If already existing game opened
    }

    //Name and id
    name->setText(player->get_name());
    idLabel->setText(QString::number(player->get_player_id()));

    //Favorite game
    this->findChild<QComboBox*>("favorite_game")->addItem(player->get_fav_game_name() + ", " + QString::number(player->get_favorite_game()));
    this->findChild<QComboBox*>("favorite_game")->setCurrentText(player->get_fav_game_name() + ", " + QString::number(player->get_favorite_game()));




    //Decription
    this->findChild<QTextEdit*>("description")->setText(player->get_description());

    if (player_id == 0) {
        on_changeButton_clicked();
        delete this->findChild<QPushButton*>("cancel");
    }
}

void PlayerCard::on_changeButton_clicked()
{
    //set all stuff editable
    name->setReadOnly(false);
    this->findChild<QTextEdit*>("description")->setReadOnly(false);

    //set favorite game combobox
    QComboBox* fav_game = this->findChild<QComboBox*>("favorite_game");
    fav_game->setDisabled(false);
    fav_game->clear();

    QSqlQuery query;
    query.prepare("SELECT game_id, name "
                  "FROM game; ");

    if (!query.exec()) {
        throw std::invalid_argument("Unable to select from `player` table");
    }

    QSqlRecord rec;
    rec = query.record();
    while(query.next()) {
        fav_game->addItem(query.value(rec.indexOf("name")).toString() + ", " + query.value(rec.indexOf("game_id")).toString());
    }
    fav_game->setCurrentText(player->get_fav_game_name() + ", " + QString::number(player->get_favorite_game()));


    //delete change button
    delete this->findChild<QPushButton*>("chagneButton");


    //add all of control buttons
    QPushButton* delete_player = new QPushButton("Удалить", this);
    delete_player->setObjectName("delete");

    QPushButton* cancel = new QPushButton("Отменить", this);
    cancel->setObjectName("cancel");

    QPushButton* save_player = new QPushButton("Сохранить", this);
    save_player->setObjectName("save");

    headerLayout->addWidget(delete_player);
    headerLayout->addWidget(cancel);
    headerLayout->addWidget(save_player);

    connect(cancel,SIGNAL(clicked()),this,SLOT(cancel_changes()));
    connect(save_player,SIGNAL(clicked()),this,SLOT(save_changes()));
    connect(delete_player,SIGNAL(clicked()),this,SLOT(delete_player()));

    //hide back button
    backButton->hide();
}

void PlayerCard::cancel_changes()
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
    this->findChild<QComboBox*>("favorite_game")->setDisabled(true);
    this->findChild<QTextEdit*>("description")->setReadOnly(true);

    set_player(player->get_player_id());
}

void PlayerCard::save_changes()
{
    player->set_name(name->text());
    player->set_description(this->findChild<QTextEdit*>("description")->toPlainText());
    player->set_favorite_game(get_last_number(this->findChild<QComboBox*>("favorite_game")->currentText()));
    player->update_DB();

    cancel_changes();

}

void PlayerCard::delete_player()
{
    QSqlQuery query;
    query.prepare(" DELETE FROM player "
                  " WHERE player_id = (?); ");
    query.bindValue(":id", idLabel->text().toInt());
    query.addBindValue(idLabel->text().toInt());

    if (!query.exec()) {
        qDebug() << query.lastError();
        throw std::invalid_argument("Unable to delete from `player` table");
    }

    backButton->show(); // show back button
    clearLayout(headerLayout,3); // delete cancel, delete and save buttons

    //set all stuff uneditable
    name->setReadOnly(true);
    this->findChild<QComboBox*>("favorite_game")->setDisabled(true);
    this->findChild<QTextEdit*>("description")->setReadOnly(true);

    //Adding back change button
    QPushButton* changeButton = new QPushButton("Изменить",this);
    changeButton->setObjectName("chagneButton");
    headerLayout->addWidget(changeButton);
    connect(changeButton,SIGNAL(clicked()),this,SLOT(on_changeButton_clicked()));

    on_backButton_clicked();

}

void PlayerCard::on_backButton_clicked()
{
    emit playerCard_backButton_clicked();
}
