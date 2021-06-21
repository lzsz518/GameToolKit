#ifndef VCVCHILDWINDOW_H
#define VCVCHILDWINDOW_H

#include <QFont>
#include <QWidget>
class QScrollBar;
class MouseEvent;
struct MainWindowStatus;
class SpriteRectangle;

using std::vector;

#define SCROLLBAR_SIZE 10

class ImageView: public QWidget
{
	Q_OBJECT
public:
    ImageView(QWidget *parent = nullptr, Qt::WindowFlags f = Qt::WindowFlags());
    ~ImageView();

public:

    void ImgUpdate(QImage *ximage);

    void SetDisplayScale(float scale);
	float GetDisplayScale();

    void SetMainWindowStatus(const MainWindowStatus &status);
public slots:
protected slots:
	void VScrollBarRangeChanged(int min, int max);
	void HScrollBarRangeChanged(int min, int max);

signals:
    void boundingboxGenerated(vector<QRect> boxs);

protected:
    virtual void paintEvent(QPaintEvent *e) override;
    virtual void closeEvent(QCloseEvent *e) override;
    virtual void resizeEvent(QResizeEvent *e) override;
    virtual void mousePressEvent(QMouseEvent *event) override;
    virtual void mouseMoveEvent(QMouseEvent *event) override;
    virtual void mouseReleaseEvent(QMouseEvent *event) override;
    virtual void wheelEvent(QWheelEvent *event) override;
    virtual void keyPressEvent(QKeyEvent *event) override;
    virtual void keyReleaseEvent(QKeyEvent *event) override;

    virtual void mouseDoubleClickEvent(QMouseEvent *event) override;

    void dragEnterEvent(QDragEnterEvent *event) override;
    void dragMoveEvent(QDragMoveEvent *event) override;
    void dragLeaveEvent(QDragLeaveEvent *event) override;
    void dropEvent(QDropEvent *event) override;
protected:
    bool left_button_down;
    bool ctrl_key_down;
    QImage *update_image;

    QScrollBar *h_scrollbar;
    QScrollBar *v_scrollbar;

    int posx,posy;
    int image_top;
    int image_left;
    QRect image_display_area;
    QRect widget_display_area;
    float display_scale;

    QFont font;
    QPainter *painter;
    MouseEvent *mouse_event;
    MainWindowStatus *mainwindow_status;
    QPoint lt,rb;

    vector<SpriteRectangle*> sprite_rect;

    vector<QRect> sprite_boundingbox;

    void AdjustScrollBar();
    void AdjustScrollBarRange();

    void DrawClient();
    void RowColumnPairToBoundingbox(const vector<SpriteRectangle*> row_column_pair, vector<QRect> &boundingbox);
    void DrawBoundingbox(QPainter &painter);
    QPoint ScreenPointToImagePoint(const QPoint &p);
    QRect FindSprite(const QPoint &p);

};

#endif
