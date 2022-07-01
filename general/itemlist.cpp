#include "./general/itemlist.h"
#include <QtSql>
#include "./general/usefulfunctions.h"

ItemList::ItemList(QWidget *parent)
    : QWidget{parent}
{
    layout = new QVBoxLayout(this);

    //Add item button
    createNewItem = new ItemButton(0);
    createNewItem->setText("Add item");
    createNewItem->setObjectName("createNewItem");
    layout->addWidget(createNewItem);

    //Scroll area setup
    itemsArea = new QScrollArea(this);
    itemsArea->setObjectName("itemsArea");
    itemsArea->setWidgetResizable(true); //IMPORTANT

    scrollAreaContent = new QWidget(itemsArea);
    itemsLayout = new QVBoxLayout();
    itemsLayout->setObjectName("itemsLayout");
    scrollAreaContent->setLayout(itemsLayout);

    itemsArea->setWidget(scrollAreaContent);

    layout->addWidget(itemsArea);

    //temsArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn); //optional

    connect(createNewItem,SIGNAL(send_ID(int)),this,SLOT(on_ItemButton_clicked(int)));
}

void ItemList::set_addItem_text(QString text)
{
    createNewItem->setText(text);
}

void ItemList::update_items_list(QString tableName)
{
    clearLayout(itemsLayout); //Start from scrath

    QSqlQuery query;
    QString queryStr;

    queryStr = "SELECT " + tableName + "_id, name "
               "FROM " + tableName + " "
               "ORDER BY " + tableName + "_id ASC;";

    if (!query.exec(queryStr)) {
        qDebug() << query.lastError();
        throw std::invalid_argument("Unable to select from table, 100");
    }

    QSqlRecord rec = query.record();

    //Fill up the table
    while(query.next()) {
        ItemButton* item = new ItemButton(query.value(rec.indexOf(tableName + "_id")).toInt());
        item->setText(query.value(rec.indexOf("name")).toString());
        itemsLayout->addWidget(item);
        item->setSizePolicy(QSizePolicy::Minimum,QSizePolicy::Fixed);
        connect(item,SIGNAL(send_ID(int)),this,SLOT(on_ItemButton_clicked(int)));
    }


    itemsLayout->addStretch();
}

void ItemList::on_ItemButton_clicked(int id)
{
    emit itemSelected(id);
}




