#include <QMdiSubWindow>
#include "spritesplitter.h"
#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    setCentralWidget(&mdi_area);

    connect(ui->actionSpriteSplitter,&QAction::triggered,this,&MainWindow::slotSpriteSplitter);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::slotSpriteSplitter()
{
    QMdiSubWindow *sub_win = new QMdiSubWindow;
    SpriteSplitter *widget = new SpriteSplitter;
    sub_win->setWidget(widget);
    mdi_area.addSubWindow(sub_win);
    sub_win->show();
}

