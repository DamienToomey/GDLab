#include "resultwindow.h"

int ResultWindow::static_windowId = 0;

ResultWindow::ResultWindow(MainWindow *mainWindow, QWidget *parent = 0)
    : QDialog(parent),
      m_mainWindow(mainWindow),
      m_widget(parent)
{

    initializeTable();

    m_tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers); // table in read mode only
    m_tableWidget->resizeColumnsToContents();
    m_tableWidget->resizeRowsToContents();


    QVBoxLayout *vLayout = new QVBoxLayout();
    vLayout->addWidget(m_tableWidget);
    setLayout(vLayout);
    setWindowFlags(Qt::Window); // adds reduce and resize buttons to title bar
    setWindowTitle(QString("Statistics (%1)").arg(static_windowId));
    static_windowId++;

    m_tableWidget->setContextMenuPolicy(Qt::ActionsContextMenu);
    QAction* copyTableAction = new QAction("Copy table");
    copyTableAction->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_C));
    connect(copyTableAction, SIGNAL(triggered()), this, SLOT(copyTable()));
    m_tableWidget->addAction(copyTableAction);

    m_tableWidget->setSizeAdjustPolicy(QAbstractScrollArea::AdjustToContents);
    m_tableWidget->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);

    show();
}

void ResultWindow::initializeTable()
{
    map<QString, QCheckBox*> gradientDescentMethodToCheckBox = m_mainWindow->gradientDescentMethodToCheckBox();
    map<QString, GradientDescent*> gradientDescentMethodToGradientDescent = m_mainWindow->gradientDescentMethodToGradientDescent();

    // Initialize table size
    int size = 0;
    map<QString, GradientDescent*>::iterator it;
    for (it = gradientDescentMethodToGradientDescent.begin(); it != gradientDescentMethodToGradientDescent.end(); ++it) {
        if (gradientDescentMethodToCheckBox[it->first]->isChecked()) {
            size++;
        }
    }
    m_tableWidget = new QTableWidget();
    m_tableWidget->setRowCount(size+1); // number of gradient descent methods

    it = gradientDescentMethodToGradientDescent.begin();
    map<QString, double> statisticLabelToValue = it->second->statisticLabelToValue();
    m_tableWidget->setColumnCount(statisticLabelToValue.size()+1); // number of statistics


    MyTableWidgetItem *newItem = new MyTableWidgetItem(QString(""));
    m_tableWidget->setItem(0, 0, newItem);

    map<QString, double>::iterator ite;
    int row = 1;
    int costValueColumn;
    for (it = gradientDescentMethodToGradientDescent.begin(); it != gradientDescentMethodToGradientDescent.end(); ++it) {
        int column = 1;
        if (gradientDescentMethodToCheckBox[it->first]->isChecked()) {
            newItem = new MyTableWidgetItem(it->second->name());
            m_tableWidget->setItem(row, 0, newItem);

            map<QString, double> statisticLabelToValue = it->second->statisticLabelToValue();
            for (ite = statisticLabelToValue.begin(); ite != statisticLabelToValue.end(); ++ite) {
                if (ite->first.contains("cost")) {
                    costValueColumn = column;
                }
                newItem = new MyTableWidgetItem(ite->first);
                newItem->setTextAlignment(Qt::AlignHCenter);
                m_tableWidget->setItem(0, column, newItem); // NOT EFFICIENT but for code readibility
                newItem = new MyTableWidgetItem(QString("%1").arg(ite->second));
                newItem->setTextAlignment(Qt::AlignHCenter);
                m_tableWidget->setItem(row, column, newItem);
                column++;
            }
            row++;
        }
    }
    m_tableWidget->sortItems(costValueColumn,  Qt::AscendingOrder);
}

void ResultWindow::copyTable()
{
   QString clipboard = QString("");   

   for (int i = 0; i < m_tableWidget->rowCount(); i++) {
       for (int j = 0; j < m_tableWidget->columnCount(); j++) {
           clipboard += m_tableWidget->item(i, j)->text() + QString(", ");
       }
       clipboard = clipboard.trimmed() + QString("\n");
   }
   QApplication::clipboard()->setText(clipboard);
}

ResultWindow::~ResultWindow()
{
    delete m_mainWindow;
}


