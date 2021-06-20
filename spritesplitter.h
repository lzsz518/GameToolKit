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

    QDockWidget *docker;
    QListWidget  *sprite_list;

    QTreeWidgetItem *boundingbox_root;
private slots:
    void slotOpen();
    void slotDrawRect(bool checked);
    void slotAccpetBoundingbox(vector<QRect> boxs);
};

#endif // SPRITESPLITTER_H
