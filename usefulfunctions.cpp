#include "usefulfunctions.h"

void clearLayout(QLayout *layout, int from)
{
    while (layout->count()>from) {
        QLayoutItem *item = layout->itemAt(from);
        if (item->layout()) {
           clearLayout(item->layout());
           delete item->layout();
           continue;
        }
        if (item->widget()) {
            delete item->widget();
            continue;
        }
        if (item->spacerItem()) {
            layout->removeItem(item);
            continue;
        }
    }
}
