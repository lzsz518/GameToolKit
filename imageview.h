#ifndef VCVCHILDWINDOW_H
#define VCVCHILDWINDOW_H

#include <QFont>
#include <QWidget>
class QScrollBar;
class MouseEvent;
struct MainWindowStatus;

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

protected:
        virtual void paintEvent(QPaintEvent *e);
        virtual void closeEvent(QCloseEvent *e);
        virtual void resizeEvent(QResizeEvent *e);
        virtual void mousePressEvent(QMouseEvent *event);
        virtual void mouseMoveEvent(QMouseEvent *event);
        virtual void mouseReleaseEvent(QMouseEvent *event);
        virtual void wheelEvent(QWheelEvent *event);
        virtual void keyPressEvent(QKeyEvent *event);
        virtual void keyReleaseEvent(QKeyEvent *event);

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


    void AdjustScrollBar();
    void AdjustScrollBarRange();

    void DrawClient();
};

#endif