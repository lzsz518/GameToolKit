#include <QIcon>
#include <QToolBar>
#include <QAction>
#include <QFileDialog>
#include "systemdefine.h"
#include "imageview.h"
#include "spritesplitter.h"
#include "ui_spritesplitter.h"

SpriteSplitter::SpriteSplitter(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::SpriteSplitter)
{
    ui->setupUi(this);

    view = new ImageView;
    setCentralWidget(view);

    connect(ui->actionOpen,&QAction::triggered,this,&SpriteSplitter::slotOpen);

    actionDrawRect = new QAction(QIcon(":/Resource/DrawRect.png"),tr("DrawLine"));
    QToolBar *toolbar = new QToolBar;
    toolbar->addAction(actionDrawRect);
    addToolBar(toolbar);
}

SpriteSplitter::~SpriteSplitter()
{
    delete ui;
}

void SpriteSplitter::slotOpen()
{
    QString fileName = QFileDialog::getOpenFileName(this,
        tr("Open Image"), "/home", tr("Image Files (*.png)"));

    if(!fileName.isEmpty())
    {
        QImage image(fileName);
        view->ImgUpdate(&image);
    }
}

void SpriteSplitter::slotDrawRect()
{
    MainWindowStatus status;
    if(actionDrawRect->isChecked())
    {
        status.CurrentDrawToolKit = DRAWRECTANGLE;
        status.CurrentColor = Qt::green;
    }
    else
    {
        status.CurrentDrawToolKit = EMPTY;
    }

    view->SetMainWindowStatus(status);
}
