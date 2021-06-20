#ifndef SPRITELIST_H
#define SPRITELIST_H

#include <QListWidget>

class SpriteList : public QListWidget
{
public:
    SpriteList();

    void addSprite(const QPixmap &pixmap, const QPoint &location);

    static QString spriteMimeType() { return QStringLiteral("image/sprite"); }

protected:
    void dragEnterEvent(QDragEnterEvent *event) override;
    void dragMoveEvent(QDragMoveEvent *event) override;
    void dropEvent(QDropEvent *event) override;
    void startDrag(Qt::DropActions supportedActions) override;
};

#endif // SPRITELIST_H
