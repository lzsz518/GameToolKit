#ifndef SPRITESPLITTER_H
#define SPRITESPLITTER_H

#include <QMainWindow>

namespace Ui {
class SpriteSplitter;
}

class ImageView;

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
private slots:
    void slotOpen();
    void slotDrawRect();
};

#endif // SPRITESPLITTER_H
