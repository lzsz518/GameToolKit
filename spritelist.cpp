#include <QDrag>
#include <QDragEnterEvent>
#include <QMimeData>
#include "spritelist.h"

SpriteList::SpriteList()
{

}


void SpriteList::dragEnterEvent(QDragEnterEvent *event)
{
    if(event->mimeData()->hasFormat(SpriteList::spriteMimeType()))
        event->accept();
    else
        event->ignore();

}

void SpriteList::dragMoveEvent(QDragMoveEvent *event)
{
    if (event->mimeData()->hasFormat(SpriteList::spriteMimeType())) {
        event->setDropAction(Qt::MoveAction);
        event->accept();
    } else {
        event->ignore();
    }
}

void SpriteList::dropEvent(QDropEvent *event)
{

}

void SpriteList::startDrag(Qt::DropActions supportedActions)
{

}
