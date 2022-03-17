#include "stylehelper.h"


QString stylehelper::getSideMenuStyle()
{
    return "QFrame{"
           "    background-color: #D9D9D9;"
           "}"
           "QListWidget{"
           "    background-color: #D9D9D9;"
           "    border: none;"
           "    font-family: 'Montserrat';"
           "    font-size: 22px;"
           "}"
           "QListWidget::item{"
           "    background-color: #D9D9D9;"
           "    color: #777777;"
           "    margin: 0px;"
           "    margin-bottom: 13px;"
           "    padding: 0px;"
           "}"
           "QListWidget::item:hover{"
           "    color: #B6B6B6;"
           "}"
           "QListWidget::item:selected{"
           "    border: none;"
           "    font-family: 'Montserrat Medium';"
           "    color: #000000;"
           "}"
           ;
}
