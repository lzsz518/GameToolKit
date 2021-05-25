#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMdiArea>
#include <QMainWindow>

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
    void slotSpriteSplitter();


private:
    Ui::MainWindow *ui;

    QMdiArea mdi_area;
//    void FindRowPair(const Mat &mat, vector<pair<int,int>> &rowpairs);
//    void FindColumnPair(const Mat &mat, vector<pair<int,int>> &columnpairs);
};
#endif // MAINWINDOW_H
