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

    void gameCard_setGame(int game_id);

    void on_newGameButton_clicked();

    void on_gameBackButton_clicked();

    void load_games_list();

    void on_gameChangeButton_clicked();

    void on_cancelGameChanges_clicked();
    void save_game();
    void delete_game();

private:
    Ui::MainWindow *ui;
    void setInterfaceStyle();

signals:
    void send_game_ID(int);
    void game_tab_selected();
};
#endif // MAINWINDOW_H
