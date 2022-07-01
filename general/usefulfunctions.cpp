#include "./general/usefulfunctions.h"
#include <cmath>

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

int get_last_number(QString string)
{
    int result = 0;
    int i = 1;
    while ((i<=string.size() && (string.at(string.size()-i).isNumber()))) {
        int temp = (string.at(string.size()-i).unicode() - 48);
        result += temp * pow(10,i-1);
        i++;
    }
    return result;
}
