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


    connect(this,SIGNAL(send_game_ID(int)),ui->gameCard,SLOT(set_game(int)));
    connect(ui->gameCard,SIGNAL(gameCard_backButton_clicked()),this,SLOT(on_gameBackButton_clicked()));

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
    show_gameCard();
}

void MainWindow::on_gameBackButton_clicked()
{
    load_games_list();
    ui->gamesWidget->setCurrentWidget(ui->gamesListWidget);
}




//Load game list to the main game tab
void MainWindow::load_games_list()
{
    clearLayout(ui->gamesList); //Start from scrath

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

    //Fill up the table
    while(query.next()) {
        ItemButton* game = new ItemButton(query.value(rec.indexOf("game_id")).toInt());
        game->setText(query.value(rec.indexOf("name")).toString());
        ui->gamesList->addWidget(game);
        connect(game,SIGNAL(send_ID(int)),ui->gameCard,SLOT(set_game(int)));
        connect(game,SIGNAL(clicked()),this,SLOT(show_gameCard()));
    }

    ui->gamesList->addStretch();
}

void MainWindow::show_gameCard()
{
    ui->gamesWidget->setCurrentWidget(ui->gameCard);
}
