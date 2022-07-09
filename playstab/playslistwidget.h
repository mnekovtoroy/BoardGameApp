#ifndef PLAYSLISTWIDGET_H
#define PLAYSLISTWIDGET_H

#include <QWidget>
#include <QtWidgets>

class PlaysListWidget : public QWidget
{
    Q_OBJECT

public:
    explicit PlaysListWidget(QWidget *parent = nullptr);

private:
    QVBoxLayout* layout;
    QPushButton* new_play_button;
    QScrollArea* plays_list_area;
    QWidget* scrollAreaContent;
    QVBoxLayout* plays_list_layout;

    void update_plays_list();

private slots:
    void new_play_button_clicked();

signals:

};

#endif // PLAYSLISTWIDGET_H
