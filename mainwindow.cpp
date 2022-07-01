#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFontDatabase>
#include <QTextEdit>
#include "./general/stylehelper.h"
#include "./gamecard/game.h"
#include "./general/itembutton.h"
#include <QDebug>
#include <QtSql>
#include <stdexcept>
#include "./general/usefulfunctions.h"



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



    //Games tab setup
    ui->gamesListWidget->update_items_list("game");
    connect(ui->gamesListWidget, SIGNAL(itemSelected(int)),this,SLOT(game_selected(int)));
    ui->gamesListWidget->set_addItem_text("Новая игра");

    connect(this,SIGNAL(send_game_ID(int)),ui->gameCard,SLOT(set_game(int)));
    connect(ui->gameCard,SIGNAL(gameCard_backButton_clicked()),this,SLOT(on_gameBackButton_clicked()));

    //Players tab setup
    ui->playersListWidget->update_items_list("player");
    connect(ui->playersListWidget,SIGNAL(itemSelected(int)),this,SLOT(player_selected(int)));
    ui->playersListWidget->set_addItem_text("Новый игрок");

    connect(this,SIGNAL(send_player_ID(int)),ui->playerCard,SLOT(set_player(int)));
    connect(ui->playerCard,SIGNAL(playerCard_backButton_clicked()),this,SLOT(on_playerBackButton_clicked()));



    //Players tab setup
    ui->playersListWidget->update_items_list("player");

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

void MainWindow::game_selected(int id)
{
    emit send_game_ID(id);
    ui->gamesWidget->setCurrentWidget(ui->gameCard);
}

void MainWindow::on_gameBackButton_clicked()
{
    ui->gamesListWidget->update_items_list("game");
    ui->gamesWidget->setCurrentWidget(ui->gamesListWidget);
}

void MainWindow::player_selected(int id)
{
    emit send_player_ID(id);
    ui->playersWidget->setCurrentWidget(ui->playerCard);
}

void MainWindow::on_playerBackButton_clicked()
{
    ui->playersListWidget->update_items_list("player");
    ui->playersWidget->setCurrentWidget(ui->playersListWidget);
}
