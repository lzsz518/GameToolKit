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


    actionDrawRect = new QAction(QIcon(":/Resource/DrawRect.png"),tr("DrawLine"));
    actionDrawRect->setCheckable(true);
    QToolBar *toolbar = new QToolBar;
    toolbar->addAction(actionDrawRect);
    addToolBar(toolbar);

    connect(ui->actionOpen,&QAction::triggered,this,&SpriteSplitter::slotOpen);
    connect(actionDrawRect,SIGNAL(triggered(bool)),this,SLOT(slotDrawRect(bool)));
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

void SpriteSplitter::slotDrawRect(bool checked)
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
