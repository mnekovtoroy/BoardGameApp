#ifndef ITEMBUTTON_H
#define ITEMBUTTON_H

#include <QPushButton>

//Button for items like games in the games tab


class ItemButton : public QPushButton
{
    Q_OBJECT
private:
    int id;

public:
    ItemButton(int id);
    int get_id();

private slots:
    void on_button_clicked();

signals:
    void send_ID(int);
};

#endif // ITEMBUTTON_H
