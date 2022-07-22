#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QListWidget>
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_SideList_currentItemChanged(QListWidgetItem *current, QListWidgetItem *previous);
    void on_gameBackButton_clicked();
    void game_selected(int id);
    void player_selected(int id);
    void play_selected(int id);
    void on_playerBackButton_clicked();
    void on_playBackButton_clicked();

private:
    Ui::MainWindow *ui;
    void setInterfaceStyle();

signals:
    void send_game_ID(int);
    void game_tab_selected();
    void send_player_ID(int);
    //void send_play_ID(int);
};
#endif // MAINWINDOW_H
