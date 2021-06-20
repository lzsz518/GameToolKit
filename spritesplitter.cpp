#include <QIcon>
#include <QToolBar>
#include <QAction>
#include <QFileDialog>
#include <QDockWidget>
#include <QListWidget>
#include <QTreeWidgetItem>
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

    boundingbox_root = new QTreeWidgetItem;
    ui->tw_boundingbox->insertTopLevelItem(0,boundingbox_root);

    connect(ui->actionOpen,&QAction::triggered,this,&SpriteSplitter::slotOpen);
    connect(actionDrawRect,SIGNAL(triggered(bool)),this,SLOT(slotDrawRect(bool)));
    connect(view,&ImageView::boundingboxGenerated,this,&SpriteSplitter::slotAccpetBoundingbox);


    docker = new QDockWidget(this);
    sprite_list = new QListWidget(this);
    docker->setWidget(sprite_list);
    addDockWidget(Qt::RightDockWidgetArea,docker);
}

SpriteSplitter::~SpriteSplitter()
{
    delete ui;
}

void SpriteSplitter::slotOpen()
{
    QString fileName = QFileDialog::getOpenFileName(this,
        tr("Open Image"), "/home/hwb/Pictures", tr("Image Files (*.png)"));

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

void SpriteSplitter::slotAccpetBoundingbox(vector<QRect> boxs)
{
    for(int i=0;i<boxs.size();++i)
    {
        QString str;
        QTreeWidgetItem *box_item = new QTreeWidgetItem;
        str = QString("%1,%2,%3,%4").arg(boxs[i].x()).arg(boxs[i].y()).arg(boxs[i].width()).arg(boxs[i].height());
        box_item->setText(0,str);
        boundingbox_root->addChild(box_item);

    }
}

