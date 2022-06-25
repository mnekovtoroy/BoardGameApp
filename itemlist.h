#ifndef ITEMLIST_H
#define ITEMLIST_H

#include <QWidget>
#include <QtWidgets>
#include "itembutton.h"

class ItemList : public QWidget
{
    Q_OBJECT
private:
    QVBoxLayout *layout;
    ItemButton *createNewItem;
    QScrollArea *itemsArea;
    QVBoxLayout *itemsLayout;
    QWidget *scrollAreaContent;
public:
    explicit ItemList(QWidget *parent = nullptr);

public slots:
    void update_items_list(QString tableName);

private slots:
    void on_ItemButton_clicked(int id);

signals:
    void itemSelected(int);
};

#endif // ITEMLIST_H
