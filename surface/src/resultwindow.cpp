#include "resultwindow.h"

ResultWindow::ResultWindow(QTableWidget *tableWidget, QWidget *parent = 0) : QDialog(parent)
{
    QVBoxLayout *vLayout = new QVBoxLayout;
    vLayout->addWidget(tableWidget);

    setLayout(vLayout);
}
