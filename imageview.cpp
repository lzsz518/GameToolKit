#include "systemdefine.h"
#include "mouseeventcreator.h"
#include "mouseevent.h"
#include <QMouseEvent>
#include <QImage>
#include <QPainter>
#include <QScrollBar>
#include "imageview.h"

#define MIN_DISPLAY_SCALE 0.1
#define MAX_DISPLAY_SCALE 10
#define DISPLAY_SCALE_STEP 0.1


ImageView::ImageView(QWidget *parent, Qt::WindowFlags f)
        :QWidget(parent,f)
{
    left_button_down = false;
    ctrl_key_down = false;
    update_image = nullptr;
    setAttribute(Qt::WA_DeleteOnClose);
    setMinimumSize(300,300);

    v_scrollbar = new QScrollBar(Qt::Vertical,this);
    h_scrollbar = new QScrollBar(Qt::Horizontal,this);

    display_scale = 1.0;


    connect(v_scrollbar,SIGNAL(valueChanged(int)),this,SLOT(repaint()));
    connect(h_scrollbar,SIGNAL(valueChanged(int)),this,SLOT(repaint()));

    setFocusPolicy(Qt::WheelFocus);

    font.setFamily(font.defaultFamily());
    font.setPointSize(10);

    painter = new QPainter(this);
    painter->setPen(Qt::red);

    mouse_event = nullptr;
    mainwindow_status = new MainWindowStatus;
}

ImageView::~ImageView()
{
    if(update_image!=nullptr)
    {
        delete update_image;
        update_image = nullptr;
    }

    for(int i=0;i<sprite_rect.size();++i)
    {
        delete sprite_rect[i];
    }
    sprite_rect.clear();

}

////////////////////////
//public function



void ImageView::SetDisplayScale(float scale)
{
    display_scale = scale;

    repaint();
}

float ImageView::GetDisplayScale()
{
    return display_scale;
}

void ImageView::SetMainWindowStatus(const MainWindowStatus &status)
{
    mainwindow_status->CurrentColor.setRgb(status.CurrentColor.rgb());
    mainwindow_status->CurrentDrawToolKit = status.CurrentDrawToolKit;
}

void ImageView::ImgUpdate(QImage *image)
{
//    if(update_image==nullptr)
//        return;

    if(update_image==nullptr)
    {
        update_image = new QImage(*image);
    }
    else
    {
        delete update_image;
        update_image = new QImage(*image);
    }

    AdjustScrollBarRange();
    update();
}


/////////////////////
//slot function

void ImageView::VScrollBarRangeChanged(int min, int max)
{
    if (max - min == 0)
        v_scrollbar->hide();
    else
        v_scrollbar->show();
}

void ImageView::HScrollBarRangeChanged(int min, int max)
{

    if (max - min == 0)
        h_scrollbar->hide();
    else
        h_scrollbar->show();
}

/////////////////////
//event function

void ImageView::paintEvent(QPaintEvent *e)
{
    if(update_image==nullptr)
    {
        QWidget::paintEvent(e);
        return;
    }

    DrawClient();

    QWidget::paintEvent(e);
}

void ImageView::closeEvent(QCloseEvent *e)
{
    QWidget::closeEvent(e);
}

void ImageView::resizeEvent(QResizeEvent *e)
{
    AdjustScrollBar();

    QWidget::resizeEvent(e);
}


void ImageView::mousePressEvent(QMouseEvent *event)
{
    if(event->button()==Qt::LeftButton)
    {
        if(mouse_event!=nullptr)
            delete mouse_event;

        left_button_down = true;
        mouse_event = MouseEventCreator::CreateMouseEvent(mainwindow_status->CurrentDrawToolKit);

        if(mouse_event != nullptr)
        {
            if(event->pos().x()>widget_display_area.x() &&
                    event->pos().x()<widget_display_area.x()+widget_display_area.width() &&
                    event->pos().y()>widget_display_area.y() &&
                    event->pos().y()<widget_display_area.y()+widget_display_area.height())
            {
                mouse_event->MousePressEvent(event);
                lt.setX((event->pos().x()-widget_display_area.x())/display_scale);
                lt.setY((event->pos().y()-widget_display_area.y())/display_scale);
            }
        }
    }
}

void ImageView::mouseMoveEvent(QMouseEvent *event)
{
    if(update_image != nullptr)
    {
        //        int x = event->x();
        //        int y = event->y();
        if(widget_display_area.contains(event->pos()))
        {
            posx = event->x()-widget_display_area.x();
            posy = event->y()-widget_display_area.y();
            if(widget_display_area.width()<image_display_area.width()*display_scale)
                posx *= (double)image_display_area.width()/(double)widget_display_area.width();
            else
            {
                posx /= display_scale;
                posx += image_display_area.left();
            }

            if(widget_display_area.height()<image_display_area.height()*display_scale)
                posy *= (double)image_display_area.height()/(double)widget_display_area.height();
            else
            {
                posy /= display_scale;
                posy += image_display_area.top();
            }


            if(left_button_down)
            {
                if(mouse_event!=nullptr)
                {
                    if(event->pos().x()>widget_display_area.x() &&
                            event->pos().x()<widget_display_area.x()+widget_display_area.width() &&
                            event->pos().y()>widget_display_area.y() &&
                            event->pos().y()<widget_display_area.y()+widget_display_area.height())
                    {
                        mouse_event->MouseMoveEvent(event);
                    }
                }
            }
            update();
        }
    }
}

void ImageView::mouseReleaseEvent(QMouseEvent *event)
{
    if(left_button_down)
    {
        if(mouse_event!=nullptr)
        {
            if(event->pos().x()>widget_display_area.x() &&
                    event->pos().x()<widget_display_area.x()+widget_display_area.width() &&
                    event->pos().y()>widget_display_area.y() &&
                    event->pos().y()<widget_display_area.y()+widget_display_area.height())
            {
                //                mouse_event->MouseReleaseEvent(event);
                rb.setX((event->pos().x()-widget_display_area.x())/display_scale);
                rb.setY((event->pos().y()-widget_display_area.y())/display_scale);
                QRect rect(lt,rb);
                SpriteRectangle *sr = FindRowAndColumnPair(*update_image,rect);

                //                QImage *img = CopyImageROI(rect ,*update_image);

                //                vector<pair<int,int>> rowpair;
                //                vector<pair<int,int>> columnpair;
                //                FindRowPair(*img, rowpair);
                //                FindColumnPair(*img,columnpair);

                //                QPainter painter(update_image);
                //                for(int i=0;i<rowpair.size();++i)
                //                {
                //                    painter.drawLine(rect.x(),rowpair[i].first+rect.y(),rect.x()+rect.width(),rowpair[i].first+rect.y());
                //                    painter.drawLine(rect.x(),rowpair[i].second+rect.y(),rect.x()+rect.width(),rowpair[i].second+rect.y());
                //                }

                //                for(int i=0;i<columnpair.size();++i)
                //                {
                //                    painter.drawLine(columnpair[i].first+rect.x(),rect.y(),columnpair[i].first+rect.x(),rect.y()+rect.height());
                //                    painter.drawLine(columnpair[i].second+rect.x(),rect.y(),columnpair[i].second+rect.x(),rect.y()+rect.height());
                //                }

                //                if(img != nullptr)
                //                    delete img;

                left_button_down = false;
                update();

                if(sr != nullptr)
                   sprite_rect.push_back(sr);
            }
        }
    }
}

void ImageView::wheelEvent(QWheelEvent *event)
{
    if(ctrl_key_down)
    {
        QPoint dp = event->angleDelta()/8;
        if(dp.y()<0)
        {
            display_scale -= DISPLAY_SCALE_STEP;
            if(display_scale < MIN_DISPLAY_SCALE)
                display_scale = MIN_DISPLAY_SCALE;
        }
        else
        {
            display_scale += DISPLAY_SCALE_STEP;
            if(display_scale>MAX_DISPLAY_SCALE)
                display_scale = MAX_DISPLAY_SCALE;
        }

        AdjustScrollBar();
        update();
    }
}

void ImageView::keyPressEvent(QKeyEvent *event)
{
//    if(event->modifiers()==Qt::ControlModifier)
    if(event->key()==Qt::Key_Control)
        ctrl_key_down = true;
}

void ImageView::keyReleaseEvent(QKeyEvent *event)
{
    if(event->key()==Qt::Key_Control)
        ctrl_key_down = false;
}


////////////////////////
//private function

void ImageView::AdjustScrollBar()
{
    int width = SCROLLBAR_SIZE; //scrollbar width
    int x = size().width() - width;
    x = x < 0 ? 0: x;
    int height = size().height()-SCROLLBAR_SIZE;
    v_scrollbar->setGeometry(x, 0, width, height);

    height = SCROLLBAR_SIZE;
    int y = size().height() - height;
    y = y < 0 ? 0: y;
    width  = size().width()-SCROLLBAR_SIZE;
    h_scrollbar->setGeometry(0, y, width, height);

    if(update_image!=nullptr)
        AdjustScrollBarRange();
}

void ImageView::AdjustScrollBarRange()
{
    if(update_image==nullptr)
        return;

    if(update_image->height()*display_scale-height()+SCROLLBAR_SIZE>0)
    {
        int pos = v_scrollbar->value();
        v_scrollbar->setRange(0,update_image->height()*display_scale-height()+SCROLLBAR_SIZE);
        v_scrollbar->setValue(pos);
    }
    else
    {
        v_scrollbar->setRange(0,0);
    }

    if(update_image->width()*display_scale-width()+SCROLLBAR_SIZE>0)
    {
        int pos = h_scrollbar->value();
        h_scrollbar->setRange(0,update_image->width()*display_scale-width()+SCROLLBAR_SIZE);
        h_scrollbar->setValue(pos);
    }
    else
    {
        h_scrollbar->setRange(0,0);
    }
}

void ImageView::DrawClient()
{
    if(update_image==nullptr)
        return;


    if(update_image->height()*display_scale-height()+SCROLLBAR_SIZE>0)
    {
        widget_display_area.setTop(0);
        widget_display_area.setHeight(height()-SCROLLBAR_SIZE);
        image_display_area.setTop(v_scrollbar->value()/display_scale);
        image_display_area.setHeight((height()-SCROLLBAR_SIZE)/display_scale);
    }
    else
    {
        widget_display_area.setTop((height()-(update_image->height()*display_scale)-SCROLLBAR_SIZE)/2);
        widget_display_area.setHeight(update_image->height()*display_scale);
        image_display_area.setTop(0);
        image_display_area.setHeight(update_image->height());
    }

    if(update_image->width()*display_scale-width()+SCROLLBAR_SIZE>0)
    {
        widget_display_area.setLeft(0);
        widget_display_area.setWidth(width()-SCROLLBAR_SIZE);
        image_display_area.setLeft(h_scrollbar->value()/display_scale);
        image_display_area.setWidth((width()-SCROLLBAR_SIZE)/display_scale);
    }
    else
    {
        widget_display_area.setLeft((width()-(update_image->width()*display_scale)-SCROLLBAR_SIZE)/2);
        widget_display_area.setWidth(update_image->width()*display_scale);
        image_display_area.setLeft(0);
        image_display_area.setWidth(update_image->width());
    }

    painter->begin(this);
    painter->drawImage(widget_display_area,*update_image,image_display_area);

    if((mouse_event != nullptr)&&(left_button_down))
        mouse_event->draw(painter);
    painter->end();
}
