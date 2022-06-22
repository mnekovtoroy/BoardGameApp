#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFontDatabase>
#include <QTextEdit>
#include <stylehelper.h>
#include "game.h"
#include "itembutton.h"
#include <QDebug>
#include <QtSql>
#include <stdexcept>
#include "usefulfunctions.h"



MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //SideList setup
    ui->SideList->addItem("Партии");
    ui->SideList->addItem("Игры");
    ui->SideList->addItem("Игроки");
    ui->SideList->addItem("Статистика");
    ui->SideList->addItem("Настройки");
    ui->SideList->setCurrentItem(ui->SideList->item(0));

    //adding fonts
    QFontDatabase::addApplicationFont(":/source/fonts/Montserrat-Medium.ttf");
    QFontDatabase::addApplicationFont(":/source/fonts/Montserrat-Regular.ttf");


    connect(this,SIGNAL(send_game_ID(int)),this,SLOT(gameCard_setGame(int)));

    load_games_list();

    setInterfaceStyle();

}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_SideList_currentItemChanged(QListWidgetItem *current, QListWidgetItem *previous)
{
    if (current->text()=="Партии") {
        ui->Window->setCurrentWidget(ui->Plays);
    } else if (current->text()=="Игры") {
        emit game_tab_selected();
        ui->Window->setCurrentWidget(ui->Games);
    } else if (current->text()=="Игроки") {
        ui->Window->setCurrentWidget(ui->Players);
    } else if (current->text()=="Статистика") {
        ui->Window->setCurrentWidget(ui->Statistics);
    } else if (current->text()=="Настройки") {
        ui->Window->setCurrentWidget(ui->Settings);
    }
}

void MainWindow::setInterfaceStyle()
{
    ui->SideMenu->setStyleSheet(stylehelper::getSideMenuStyle());
}

void MainWindow::on_newGameButton_clicked()
{
    emit send_game_ID(0);
    on_gameChangeButton_clicked();
    delete ui->gameCard->findChild<QPushButton*>("cancel_game");
    delete ui->gameCard->findChild<QPushButton*>("change_game");

}

//Set game
void MainWindow::gameCard_setGame(int game_id)
{
    //ui->gameID->setText(QString::number(game_id));
    Game* game;

    if (game_id == 0) {
        game = new Game; //If "New Game" Button pressed
        game->update_DB();
    } else {
        game = new Game(game_id); //If already existing game opened
    }

    //Header
    ui->gameName->setText(game->get_name());
    ui->gameName->setReadOnly(true);
    ui->gameID->setText(QString::number(game->get_game_id()));
    QPushButton *changeGame = new QPushButton("Изменить",ui->gameCard);
    changeGame->setObjectName("change_game");
    ui->gameHeaderLayout->addWidget(changeGame);
    connect(changeGame,SIGNAL(clicked()),this,SLOT(on_gameChangeButton_clicked()));

    //Clearing right and left layouts just to be sure
    clearLayout(ui->gameLeftColumnLayout);
    clearLayout(ui->gameRightColumnLayout);

    //Number of players setup
    QHBoxLayout *num_of_pl_Layout = new QHBoxLayout();

    QLabel *num_of_players_label = new QLabel("Кол-во игроков: ");
    num_of_pl_Layout->addWidget(num_of_players_label);

    QLineEdit *min_players = new QLineEdit(ui->gameCard);
    min_players->setText(QString::number(game->get_min_players()));
    min_players->setReadOnly(true);
    num_of_pl_Layout->addWidget(min_players);
    min_players->setObjectName("min_players");

    QLabel *deflabel = new QLabel("-");
    num_of_pl_Layout->addWidget(deflabel);

    QLineEdit *max_players = new QLineEdit();
    max_players->setText(QString::number(game->get_max_players()));
    max_players->setReadOnly(true);
    num_of_pl_Layout->addWidget(max_players);
    max_players->setObjectName("max_players");

    ui->gameRightColumnLayout->addLayout(num_of_pl_Layout);

    //Wintype setup
    QHBoxLayout *wintype_Layout = new QHBoxLayout();

    QLabel *wintype_Label = new QLabel("Тип победы: ", ui->gameCard);
    wintype_Layout->addWidget(wintype_Label);

    QLabel *wintype = new QLabel(game->get_win_type(),ui->gameCard);
    wintype_Layout->addWidget(wintype);

    ui->gameRightColumnLayout->addLayout(wintype_Layout);

    //Description setup
    if (game->get_description()!="") {
        QTextEdit *description = new QTextEdit(ui->gameCard);
        description->setText(game->get_description());
        description->setReadOnly(true);
        ui->gameLeftColumnLayout->addWidget(description);
        description->setObjectName("game_description");
    }

    //Spacers setups
    ui->gameRightColumnLayout->addStretch();
    ui->gameLeftColumnLayout->addStretch();


    ui->gamesWidget->setCurrentWidget(ui->gameCard);
}

void MainWindow::on_gameBackButton_clicked()
{
    ui->gamesWidget->setCurrentWidget(ui->gamesListWidget);
    //Clear columns so they wont duplicate next time
    clearLayout(ui->gameRightColumnLayout);
    clearLayout(ui->gameLeftColumnLayout);
    //delete ui->gameHeaderLayout->itemAt(3);

    delete ui->gameCard->findChild<QPushButton*>("change_game");

}


//Load game list to the main game tab
void MainWindow::load_games_list()
{
    clearLayout(ui->gamesList);

    QSqlQuery query;
    QString queryStr;

    queryStr = "SELECT game.game_id, game.name "
               "FROM game "
               "ORDER BY game_id ASC;";

    if (!query.exec(queryStr)) {
        qDebug() << query.lastError();
        throw std::invalid_argument("Unable to select from `game` table");
    }

    QSqlRecord rec = query.record();

    while(query.next()) {
        ItemButton* game = new ItemButton(query.value(rec.indexOf("game_id")).toInt());
        game->setText(query.value(rec.indexOf("name")).toString());
        ui->gamesList->addWidget(game);
        connect(game,SIGNAL(send_ID(int)),this,SLOT(gameCard_setGame(int)));
    }

    ui->gamesList->addStretch();


}


//Change mode on a game card
void MainWindow::on_gameChangeButton_clicked()
{
    ui->gameName->setReadOnly(false);
    ui->gameCard->findChild<QLineEdit*>("min_players")->setReadOnly(false);
    ui->gameCard->findChild<QLineEdit*>("max_players")->setReadOnly(false);
    ui->gameCard->findChild<QTextEdit*>("game_description")->setReadOnly(false);

    delete ui->gameCard->findChild<QPushButton*>("change_game");

    QPushButton* delete_game = new QPushButton("Удалить", ui->gameCard);
    delete_game->setObjectName("delete_game");

    QPushButton* cancel = new QPushButton("Отменить", ui->gameCard);
    cancel->setObjectName("cancel_game");

    QPushButton* save_game = new QPushButton("Сохранить", ui->gameCard);
    save_game->setObjectName("save_game");

    ui->gameHeaderLayout->addWidget(delete_game);
    ui->gameHeaderLayout->addWidget(cancel);
    ui->gameHeaderLayout->addWidget(save_game);
    connect(cancel,SIGNAL(clicked()),this,SLOT(on_cancelGameChanges_clicked()));
    connect(save_game,SIGNAL(clicked()),this,SLOT(save_game()));
    connect(delete_game,SIGNAL(clicked()),this,SLOT(delete_game()));


    ui->gameBackButton->hide();


}

//Discard changes
void MainWindow::on_cancelGameChanges_clicked()
{
    ui->gameBackButton->show();
    clearLayout(ui->gameHeaderLayout,3);
    gameCard_setGame(ui->gameID->text().toInt());
}

//Save changes
void MainWindow::save_game()
{
    Game game(ui->gameID->text().toInt());
    game.set_name(ui->gameName->text());
    game.set_description(ui->gameCard->findChild<QTextEdit*>("game_description")->toPlainText());
    game.set_min_players(ui->gameCard->findChild<QLineEdit*>("min_players")->text().toInt());
    game.set_max_players(ui->gameCard->findChild<QLineEdit*>("max_players")->text().toInt());
    game.update_DB();

    on_cancelGameChanges_clicked();
    load_games_list();
}

//Delete game card
void MainWindow::delete_game()
{
    QSqlQuery query;
    query.prepare(" DELETE FROM game "
                  " WHERE game_id = (?); ");
    query.bindValue(":id", ui->gameID->text().toInt());
    query.addBindValue(ui->gameID->text().toInt());

    if (!query.exec()) {
        qDebug() << query.lastError();
        throw std::invalid_argument("Unable to delete from `game` table");
    }
    load_games_list();
    ui->gameBackButton->show();
    clearLayout(ui->gameHeaderLayout,3);


    on_gameBackButton_clicked();
}

