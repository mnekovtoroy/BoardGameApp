#ifndef NEWPLAYDIALOG_H
#define NEWPLAYDIALOG_H

#include <QDialog>
#include <QtWidgets>

class NewPlayDialog : public QDialog
{
    Q_OBJECT
private:
    QVBoxLayout* layout;
    QHBoxLayout* game_date_layout;
    QVBoxLayout* players;
    QHBoxLayout* buttonsLayout;
    QLabel* dateLabel;
    QLabel* gameLabel;
    QComboBox* game;
    QLineEdit* date;
    QPushButton* OK_button;
    QPushButton* cancel_button;

public:
    NewPlayDialog();
private slots:
    void game_selected(int index);
    void addPlayer();
    void deletePlayer();
    void OK_button_clicked();
};

#endif // NEWPLAYDIALOG_H
