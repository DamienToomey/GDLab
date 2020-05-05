#include "mytablewidgetitem.h"

#include <QDebug>

MyTableWidgetItem::MyTableWidgetItem()
    : QTableWidgetItem()
{
}

MyTableWidgetItem::MyTableWidgetItem(const QString &text)
    : QTableWidgetItem(text)
{
}

bool MyTableWidgetItem::operator<(const QTableWidgetItem &other) const {
    if (other.row() == 0) { // do not sort row 0 which contains labels
        return false;
    }
    return text().toDouble() < other.text().toDouble();
    //return QTableWidgetItem::operator<(other);
}
