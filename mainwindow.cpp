#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFontDatabase>
#include <stylehelper.h>


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

