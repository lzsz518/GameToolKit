#include <QIcon>
#include <QLabel>
#include <QTimer>
#include <QToolBar>
#include <QAction>
#include <QFileDialog>
#include <QVariant>
#include <QDockWidget>
#include <QListWidget>
#include <QTreeWidgetItem>
#include <QListWidgetItem>
#include <QContextMenuEvent>
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

    createMenu();


    actionDrawRect = new QAction(QIcon(":/Resource/DrawRect.png"),tr("DrawLine"));
    actionDrawRect->setCheckable(true);
    QToolBar *toolbar = new QToolBar;
    toolbar->addAction(actionDrawRect);
    addToolBar(toolbar);

    boundingbox_root = new QTreeWidgetItem;
    boundingbox_root->setText(0,"Bounding Box");
    ui->tw_boundingbox->addTopLevelItem(boundingbox_root);

    spritesheet_root = new QTreeWidgetItem;
    spritesheet_root->setText(0,"Sprite Sheet");
    spritesheet_root->setData(0,SPRITE_TYPE, SPRITE_ROOT);
    ui->tw_sprites->addTopLevelItem(spritesheet_root);


    connect(ui->actionOpen,&QAction::triggered,this,&SpriteSplitter::slotOpen);
    connect(actionDrawRect,SIGNAL(triggered(bool)),this,SLOT(slotDrawRect(bool)));
    connect(view,&ImageView::boundingboxGenerated,this,&SpriteSplitter::slotAccpetBoundingbox);
    connect(view,&ImageView::spriteSelected,this,&SpriteSplitter::slotSpriteSelected);
    connect(ui->tw_sprites, &QTreeWidget::customContextMenuRequested,this,&SpriteSplitter::slotSpritesheetContextMenuRequested);


    docker = new QDockWidget(this);
    sprite_list = new QListWidget(this);
    docker->setWidget(sprite_list);
    addDockWidget(Qt::RightDockWidgetArea,docker);
    sprite_list->setIconSize(QSize(64,64));

    sprite_timer = new QTimer;
    connect(sprite_timer,&QTimer::timeout,this,&SpriteSplitter::slotSpriteTimer);

    sprite_label = new QLabel;
    sprite_label->setMinimumSize(128,128);
    sprite_label->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);

    ui->tw_sprites->setContextMenuPolicy(Qt::CustomContextMenu);
}

SpriteSplitter::~SpriteSplitter()
{
    delete ui;
    delete sprite_timer;
}

void SpriteSplitter::createMenu()
{
    actionAddSpriteSheet = new QAction("Add Sprite sheet");
    menuAddSpriteSheet = new QMenu;
    menuAddSpriteSheet->addAction(actionAddSpriteSheet);

    actionShowSpriteSheet = new QAction("Show Sprite sheet");
    menuShowSpriteSheet = new QMenu;
    menuShowSpriteSheet->addAction(actionShowSpriteSheet);

    connect(actionAddSpriteSheet,&QAction::triggered,this,&SpriteSplitter::slotCreateSpriteSheet);
    connect(actionShowSpriteSheet,&QAction::triggered,this,&SpriteSplitter::slotShowSpriteSheet);
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

void SpriteSplitter::slotSpriteSelected(QImage *img, QRect rect)
{


    QTreeWidgetItem *item = ui->tw_sprites->currentItem();

    if(item->data(0,SPRITE_TYPE)==SPRITE_ITEM)
    {
        QTreeWidgetItem *childItem = new QTreeWidgetItem;
        QString str = QString("%1,%2,%3,%4").arg(rect.x()).arg(rect.y()).arg(rect.width()).arg(rect.height());
        childItem->setData(0,Qt::DisplayRole,str);
        childItem->setData(0,SPRITE_POS,rect);
        QVariant var = QVariant::fromValue(img);
        childItem->setData(0,SPRITE_IMG,var);
        item->addChild(childItem);
    }
}

void SpriteSplitter::slotSpritesheetContextMenuRequested(const QPoint &pos)
{
    QModelIndex index = ui->tw_sprites->indexAt(pos);

    QVariant var = index.data(SPRITE_TYPE);
    if(var.toInt() == SPRITE_ROOT)
    {
        menuAddSpriteSheet->exec(QCursor::pos());
    }

    if(var.toInt() == SPRITE_ITEM)
    {
        menuShowSpriteSheet->exec(QCursor::pos());
    }
}

void SpriteSplitter::slotCreateSpriteSheet()
{
    QTreeWidgetItem *item = new QTreeWidgetItem;
    int childcount = spritesheet_root->childCount();
    QString str = QString("SpriteSheet%1").arg(childcount);
    item->setData(0,Qt::DisplayRole,str);
    item->setData(0,SPRITE_TYPE,SPRITE_ITEM);
    spritesheet_root->addChild(item);
}

void SpriteSplitter::slotShowSpriteSheet()
{
    QTreeWidgetItem *item = ui->tw_sprites->currentItem();
    if(item->data(0,SPRITE_TYPE)==SPRITE_ITEM)
    {
        sprite_list->clear();
        int childcount = item->childCount();
        for(int i=0; i< childcount;++i)
        {
            QImage *img = item->child(i)->data(0,SPRITE_IMG).value<QImage*>();
                QPixmap pixmap = QPixmap::fromImage(*img);
                QListWidgetItem *pieceItem = new QListWidgetItem;
                pieceItem->setIcon(QIcon(pixmap));
                pieceItem->setData(Qt::UserRole, QVariant(pixmap));
            //    pieceItem->setData(Qt::UserRole+1, location);
                pieceItem->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsDragEnabled);
                sprite_list->addItem(pieceItem);
        }
        current_sprite = 0;
        sprite_timer->start(300);
        sprite_label->show();
    }
}

void SpriteSplitter::slotSpriteTimer()
{
    if(sprite_list->count()==0)
        return;

    QListWidgetItem *item = sprite_list->item(current_sprite);
    QPixmap pixmap = item->icon().pixmap(item->icon().availableSizes()[0]);
    sprite_label->setPixmap(pixmap);

    ++current_sprite;
    if(current_sprite % sprite_list->count() ==0)
        current_sprite = 0;
}






























