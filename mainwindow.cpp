#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFontDatabase>
#include <stylehelper.h>
#include "game.h"


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


void MainWindow::on_gameBackButton_pressed()
{
    ui->gamesWidget->setCurrentWidget(ui->gamesListWidget);
}

void MainWindow::on_newGameButton_clicked()
{
    emit send_game_ID(1);
    ui->gamesWidget->setCurrentWidget(ui->gameCard);
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

    QHBoxLayout *num_of_pl_Layout = new QHBoxLayout(ui->gameCard);

    QLabel *num_of_players_label = new QLabel("Кол-во игроков: ", ui->gameCard);
    num_of_pl_Layout->addWidget(num_of_players_label);

    QLineEdit *min_players = new QLineEdit(ui->gameCard);
    min_players->setText(QString::number(game->get_min_players()));
    min_players->setReadOnly(true);
    num_of_pl_Layout->addWidget(min_players);

    QLabel *deflabel = new QLabel("-", ui->gameCard);
    num_of_pl_Layout->addWidget(deflabel);

    QLineEdit *max_players = new QLineEdit(ui->gameCard);
    max_players->setText(QString::number(game->get_max_players()));
    max_players->setReadOnly(true);
    num_of_pl_Layout->addWidget(max_players);

    ui->gameRightColomnLayout->addLayout(num_of_pl_Layout);
}


