#ifndef RESULTWINDOW_H
#define RESULTWINDOW_H

#include <QtWidgets>

#include "mainwindow.h"
#include "mytablewidgetitem.h"

class ResultWindow : public QDialog
{

    Q_OBJECT

public:
    ResultWindow(MainWindow *mainWindow, QWidget *parent);
    ~ResultWindow();
    void initializeTable();

public Q_SLOTS:
    void copyTable();

private:
    MainWindow *m_mainWindow;
    QWidget *m_widget;
    QTableWidget *m_tableWidget;
    static int static_windowId;
};

#endif // RESULTWINDOW_H
