#ifndef SPRITESPLITTER_H
#define SPRITESPLITTER_H

#include <vector>
#include <QMainWindow>

namespace Ui {
class SpriteSplitter;
}

using std::vector;

class ImageView;
class QDockWidget;
class QLabel;
class QTimer;
class QListWidget;
class QTreeWidgetItem;

class SpriteSplitter : public QMainWindow
{
    Q_OBJECT

public:
    explicit SpriteSplitter(QWidget *parent = nullptr);
    ~SpriteSplitter();

private:
    Ui::SpriteSplitter *ui;

    ImageView *view;

    QAction *actionDrawRect;
    QAction *actionAddSpriteSheet;
    QAction *actionWriteJson;
    QAction *actionWriteSprite;
    QAction *actionShowSpriteSheet;
    QAction *actionEditSpriteSheet;

    QMenu *menuSpriteSheetRoot;
    QMenu *menuSpriteSheet;

    QDockWidget *docker;
    QTimer *sprite_timer;
    QString spritefilename;

    int current_sprite;

    QTreeWidgetItem *boundingbox_root;

    QTreeWidgetItem *spritesheet_root;

    void createMenu();
private slots:
    void slotOpen();
    void slotDrawRect(bool checked);
    void slotAccpetBoundingbox(vector<QRect> boxs);
    void slotSpriteSelected(QImage *img, QRect rect);
    void slotSpritesheetContextMenuRequested(const QPoint &pos);
    void slotCreateSpriteSheet();
    void slotShowSpriteSheet();
    void slotEditSpriteSheet();
    void slotSpriteTimer();
    void slotWriteJson();
    void slotWriteSprite();
    void slotReplacePixelColor();
    void slotSaveAs();
};


#endif // SPRITESPLITTER_H
