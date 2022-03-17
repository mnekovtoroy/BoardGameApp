#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFontDatabase>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->SideList->addItem("Партии");
    ui->SideList->addItem("Игры");
    ui->SideList->addItem("Игроки");
    ui->SideList->addItem("Статистика");
    ui->SideList->addItem("Настроечки");

    int id = QFontDatabase::addApplicationFont(":/source/fonts/Montserrat-Regular.ttf");
    qDebug() << QFontDatabase::applicationFontFamilies(id).at(0);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_SideList_currentItemChanged(QListWidgetItem *current, QListWidgetItem *previous)
{
    current->setForeground(QColor::fromRgb(0, 0, 0));
    if (previous!=nullptr) {
        previous->setForeground(QColor::fromRgb(119, 119, 119));
    }
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

