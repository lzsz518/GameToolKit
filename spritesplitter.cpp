#include <QRgb>
#include <QIcon>
#include <QLabel>
#include <QTimer>
#include <QToolBar>
#include <QAction>
#include <QFileDialog>
#include <QVariant>
#include <QDockWidget>
#include <QListWidget>
#include <QMessageBox>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QPainter>
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
    connect(ui->actionRelpaceColor,&QAction::triggered,this,&SpriteSplitter::slotReplacePixelColor);
    connect(ui->actionSave_as,&QAction::triggered,this,&SpriteSplitter::slotSaveAs);
    connect(ui->actionSelectAll,&QAction::triggered,this,&SpriteSplitter::slotSelectAllSprites);
    sprite_timer = new QTimer;
    connect(sprite_timer,&QTimer::timeout,this,&SpriteSplitter::slotSpriteTimer);

    ui->tw_sprites->setContextMenuPolicy(Qt::CustomContextMenu);
    ui->tw_sprites->expandAll();
}

SpriteSplitter::~SpriteSplitter()
{
    delete ui;
    delete sprite_timer;
}

void SpriteSplitter::createMenu()
{
    actionAddSpriteSheet = new QAction("Add Sprite sheet");
    actionWriteJson = new QAction("Write Json");
    actionWriteSprite = new QAction("Write Sprite");
    actionWriteSpriteSameSize = new QAction("Write Sprite Same Size");
    menuSpriteSheetRoot = new QMenu;
    menuSpriteSheetRoot->addAction(actionAddSpriteSheet);
    menuSpriteSheetRoot->addAction(actionWriteJson);
    menuSpriteSheetRoot->addAction(actionWriteSprite);
    menuSpriteSheetRoot->addAction(actionWriteSpriteSameSize);

    actionShowSpriteSheet = new QAction("Show");
    actionEditSpriteSheet = new QAction("Edit");
    menuSpriteSheet = new QMenu;
    menuSpriteSheet->addAction(actionShowSpriteSheet);
    menuSpriteSheet->addAction(actionEditSpriteSheet);

    connect(actionAddSpriteSheet,&QAction::triggered,this,&SpriteSplitter::slotCreateSpriteSheet);
    connect(actionWriteJson,&QAction::triggered,this,&SpriteSplitter::slotWriteJson);
    connect(actionWriteSprite,&QAction::triggered,this,&SpriteSplitter::slotWriteSprite);
    connect(actionWriteSpriteSameSize,&QAction::triggered,this,&SpriteSplitter::slotWriteSpriteSameSize);
    connect(actionShowSpriteSheet,&QAction::triggered,this,&SpriteSplitter::slotShowSpriteSheet);
    connect(actionEditSpriteSheet,&QAction::triggered,this,&SpriteSplitter::slotEditSpriteSheet);
}

void SpriteSplitter::slotOpen()
{
    QString fileName = QFileDialog::getOpenFileName(this,
        tr("Open Image"), "/home/hwb/Pictures", tr("Image Files (*.png)"));

    if(!fileName.isEmpty())
    {
        QImage image(fileName);
        AddBorderForImage(image);
        view->ImgUpdate(&image);
        spritefilename = fileName;
        this->setWindowTitle(spritefilename);
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
        ui->tw_sprites->expandAll();
    }
}

void SpriteSplitter::slotSpritesheetContextMenuRequested(const QPoint &pos)
{
    QModelIndex index = ui->tw_sprites->indexAt(pos);

    QVariant var = index.data(SPRITE_TYPE);
    if(var.toInt() == SPRITE_ROOT)
    {
        menuSpriteSheetRoot->exec(QCursor::pos());
    }

    if(var.toInt() == SPRITE_ITEM)
    {
        menuSpriteSheet->exec(QCursor::pos());
    }
}

void SpriteSplitter::slotCreateSpriteSheet()
{
    QTreeWidgetItem *item = new QTreeWidgetItem;
    int childcount = spritesheet_root->childCount();
    QString str = QString("SpriteSheet%1").arg(childcount);
    item->setData(0,Qt::DisplayRole,str);
    item->setData(0,SPRITE_TYPE,SPRITE_ITEM);
    item->setFlags(item->flags() | Qt::ItemIsEditable);
    ui->tw_sprites->expandAll();
    spritesheet_root->addChild(item);
}

void SpriteSplitter::slotShowSpriteSheet()
{
    QTreeWidgetItem *item = ui->tw_sprites->currentItem();
    if(item->data(0,SPRITE_TYPE)==SPRITE_ITEM)
    {
        ui->sprite_list->clear();
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
                ui->sprite_list->addItem(pieceItem);
        }
        current_sprite = 0;
        sprite_timer->start(300);
    }
}

void SpriteSplitter::slotEditSpriteSheet()
{
    QTreeWidgetItem *item = ui->tw_sprites->currentItem();
    if(item->data(0,SPRITE_TYPE)==SPRITE_ITEM)
    {
        ui->tw_sprites->editItem(item,0);
    }

}

void SpriteSplitter::slotSpriteTimer()
{
    if(ui->sprite_list->count()==0)
        return;

    QListWidgetItem *item = ui->sprite_list->item(current_sprite);
    QPixmap pixmap = item->icon().pixmap(item->icon().availableSizes()[0]);
    ui->sprite_label->setPixmap(pixmap);

    ++current_sprite;
    if(current_sprite % ui->sprite_list->count() ==0)
        current_sprite = 0;
}

void SpriteSplitter::slotWriteJson()
{
    if(spritefilename.isEmpty())
    {
        QMessageBox::information(nullptr,"","Please open a image.");
        return;
    }

    QFileInfo fileinfo(spritefilename);
    QString basename = fileinfo.completeBaseName();
    QFile file(QApplication::applicationDirPath()+"/"+basename+".json");
    if(!file.open(QIODevice::WriteOnly))
    {
        QMessageBox::information(nullptr,"","Json file create failed.");
    }

    QJsonDocument jdoc;
    QJsonObject jobj;
    QJsonArray jarr;
    jobj["SpriteFileName"] = spritefilename;

    QTreeWidgetItem *topitem = ui->tw_sprites->topLevelItem(0);
    for(size_t i=0; i<topitem->childCount();++i)
    {
        QJsonObject jspritesheet;
        QJsonArray jsheetarray;
        QTreeWidgetItem *childitem = topitem->child(i);
        jspritesheet["Name"] = childitem->text(0);

        for(size_t j=0; j<childitem->childCount(); ++j)
        {
            QJsonObject jrectobj;
            QTreeWidgetItem *rectitem = childitem->child(j);
            QRect rect = rectitem->data(0,SPRITE_POS).toRect();
            jrectobj["left"] = QString("%1").arg(rect.x());
            jrectobj["Top"] = QString("%1").arg(rect.y());
            jrectobj["Width"] = QString("%1").arg(rect.width());
            jrectobj["Height"] = QString("%1").arg(rect.height());
            jsheetarray.append(jrectobj);
        }
        jspritesheet["Rect"] = jsheetarray;
        jarr.append(jspritesheet);
 }
    jobj["SpriteSheets"] = jarr;

    jdoc.setObject(jobj);
    file.write(jdoc.toJson(QJsonDocument::Indented));
    file.close();
}

void SpriteSplitter::slotWriteSprite()
{
    QString foldername = QFileDialog::getExistingDirectory();
    if(foldername.isEmpty())
        return;

    QTreeWidgetItem *topitem = ui->tw_sprites->topLevelItem(0);
    for(size_t i=0; i<topitem->childCount();++i)
    {
        QTreeWidgetItem *childitem = topitem->child(i);
        QString spritename_root = childitem->text(0);

        int bits = QString("%1").arg(childitem->childCount()).size();
        for(size_t j=0; j<childitem->childCount(); ++j)
        {
            QTreeWidgetItem *rectitem = childitem->child(j);
            QImage *img = rectitem->data(0,SPRITE_IMG).value<QImage*>();
            QString spritename_child = foldername + "/" + spritename_root + QString("%1").arg(j,bits) + ".png";
            img->save(spritename_child,"PNG",100);
        }
    }
}

void SpriteSplitter::slotWriteSpriteSameSize()
{
    QString foldername = QFileDialog::getExistingDirectory();
    if(foldername.isEmpty())
        return;

    QTreeWidgetItem *topitem = ui->tw_sprites->topLevelItem(0);
    int max_width = 0;
    int max_height = 0;

    for(size_t i=0; i<topitem->childCount();++i)
    {
        QTreeWidgetItem *childitem = topitem->child(i);
        QString spritename_root = childitem->text(0);

        int bits = QString("%1").arg(childitem->childCount()).size();

        FindMaxWH(childitem,max_width,max_height);
        int gap = 8;
        /*
        QImage *bigImg = new QImage(QSize((max_width)*childitem->childCount()+
            gap* (childitem->childCount()+1),max_height),
            childitem->child(0)->data(0,SPRITE_IMG).value<QImage*>()->format());
        */

		max_width += gap;
        QImage* bigImg = new QImage(QSize((max_width)*childitem->childCount(), max_height),
            childitem->child(0)->data(0, SPRITE_IMG).value<QImage*>()->format());

        bigImg->fill(QColor(0,0,0,0));

        for(size_t j=0; j<childitem->childCount(); ++j)
        {
            QTreeWidgetItem *rectitem = childitem->child(j);
            QImage *img = rectitem->data(0,SPRITE_IMG).value<QImage*>();
            QString spritename_child = foldername + "/" + spritename_root + QString("%1").arg(j,bits) + ".png";
            if(img->width()<max_width || img->height()<max_height)
            {
                int newwidth = 0;
                int newheight = 0;
                if(img->width()<max_width)
                    newwidth = max_width;
                else
                    newwidth = img->width();
                if(img->height()<max_height)
                    newheight = max_height;
                else
                    newheight = max_height;

                int widthoffset = (newwidth - img->width())*0.5;
                int heightoffset = (newheight - img->height())*0.5;

                int byteprepix = img->depth()>>3;
                for(int line = 0; line < img->height(); ++line)
                {
//                    memcpy(tempimg->scanLine(line + heightoffset) + (widthoffset * byteprepix), img->constScanLine(line),img->bytesPerLine());
                    //memcpy(bigImg->scanLine(line + heightoffset) + (j*max_width*byteprepix +(widthoffset * byteprepix + (j+1)*gap*byteprepix)), img->constScanLine(line),img->bytesPerLine());
                    memcpy(bigImg->scanLine(line + heightoffset) + (j * max_width * byteprepix + (widthoffset * byteprepix)), img->constScanLine(line), img->bytesPerLine());

                }
            }
            else
            {
                int byteprepix = img->depth()>>3;
                for(int line = 0; line < img->height(); ++line)
                {
                    //memcpy(bigImg->scanLine(line) + (j*max_width * byteprepix + (j+1)*gap*byteprepix), img->constScanLine(line),img->bytesPerLine());
                    //memcpy(bigImg->scanLine(line) + (j * max_width * byteprepix + (j + 1) * gap * byteprepix), img->constScanLine(line), img->bytesPerLine());
                    memcpy(bigImg->scanLine(line) + (j * max_width * byteprepix), img->constScanLine(line), img->bytesPerLine());
                }
            }
        }
        bigImg->save(foldername  + "/" + spritename_root + QString("%1").arg(i,bits) + ".png", "PNG", 100);
        delete bigImg;
    }
}

void SpriteSplitter::slotReplacePixelColor()
{
    view->ReplacePixelColor(QColor(),QColor(255,255,255,255));
}

void SpriteSplitter::slotSaveAs()
{
    QString filename = QFileDialog::getSaveFileName();
    if(filename.isEmpty())
        return;

    if(view != nullptr)
        view->SaveAs(filename);
}

void SpriteSplitter::slotSelectAllSprites()
{
    if(view != nullptr)
    {
        vector<QImage*> imgs;
        vector<QRect> rects;
        view->getAllSprite(imgs,rects);
        for(int i=0; i< imgs.size(); ++i)
        {
            slotSpriteSelected(imgs[i], rects[i]);
        }
    }
}


void SpriteSplitter::AddBorderForImage(QImage& img)
{
    int borderWidth = 30;
    QColor borderColor(0,0,0,0);

    int newWidth = img.width() + (2 * borderWidth);
    int newHeight = img.height() + (2 * borderWidth);

    QImage borderedImage(newWidth, newHeight, img.format());
    borderedImage.fill(borderColor);

    QPainter painter(&borderedImage);
    painter.drawImage(borderWidth, borderWidth, img);
    painter.end();

    img = borderedImage;
}





















