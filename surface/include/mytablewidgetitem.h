#ifndef MYTABLEWIDGETITEM_H
#define MYTABLEWIDGETITEM_H

#include <QTableWidgetItem>

class MyTableWidgetItem : public QTableWidgetItem
{
public:
    MyTableWidgetItem();
    MyTableWidgetItem(const QString &text);
    virtual bool operator<(const QTableWidgetItem &other) const;
};

#endif // MYTABLEWIDGETITEM_H
