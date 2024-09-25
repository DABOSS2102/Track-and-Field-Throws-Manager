#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "qtablewidget.h"
#include <QMainWindow>
#include <vector>

using namespace std;

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_getInfoButton_clicked();

    void on_sortByDD_currentIndexChanged(int index);

    void on_throwTypeDD_currentIndexChanged(int index);

    void on_startDate_userDateChanged(const QDate &date);

    void on_endDate_userDateChanged(const QDate &date);

    void on_dataTable_itemDoubleClicked(QTableWidgetItem *item);

    void on_displayChoice_currentIndexChanged(int index);

    void setTableRowsColsHeaders(int rows, int cols, QStringList headers, vector<QString> data);

    void buildTable();

    void on_minDistance_valueChanged(double arg1);

    void on_maxDistance_valueChanged(double arg1);

    void on_throwTypeDD_currentTextChanged(const QString &arg1);

    void setDefaultValues();

    void enableAdds();

    void on_addInfo_clicked();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
