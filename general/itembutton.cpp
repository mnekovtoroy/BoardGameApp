#include "./general/itembutton.h"

ItemButton::ItemButton(int id)
{
    this->id = id;
    connect(this,SIGNAL(clicked()),this,SLOT(on_button_clicked()));

}

int ItemButton::get_id()
{
    return this->id;
}

void ItemButton::on_button_clicked()
{
    emit send_ID(id);
}
