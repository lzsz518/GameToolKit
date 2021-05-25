#include <QPainter>
#include <QPaintDevice>
#include <QMouseEvent>
#include "mouseevent.h"

MouseEvent::MouseEvent(QWidget *parent) : QWidget(parent)
{
    mouse_data = nullptr;
}

MouseEvent::~MouseEvent()
{
    if(mouse_data!=nullptr)
        delete mouse_data;
}

void MouseEvent::MousePressEvent(QMouseEvent *event)
{

}

void MouseEvent::MouseMoveEvent(QMouseEvent *event)
{

}

void MouseEvent::MouseReleaseEvent(QMouseEvent *event)
{

}

void MouseEvent::draw(QPainter *painter)
{
    MouseEventData_Shape *data = static_cast<MouseEventData_Shape*>(GetData());
    QPen *pen = nullptr;
    if(data!=nullptr)
    {
        data->GetPen(pen);
    }
    if(pen==nullptr)
        data->SetPen(painter->pen());
    else
        painter->setPen(*pen);
}

MouseEventData* MouseEvent::GetData()
{
    return mouse_data;
}

void MouseEvent::InitializeData(MouseEventData *d)
{
    mouse_data = d;
}

MouseEvent_Line::MouseEvent_Line()
{
    InitializeData(new MouseEventData_Line);
}

MouseEvent_Line::~MouseEvent_Line()
{

}

void MouseEvent_Line::MousePressEvent(QMouseEvent *event)
{
    MouseEventData_Line *data = static_cast<MouseEventData_Line*>(GetData());
    if(data!=nullptr)
    {
        data->SetPoint1(event->pos());
        data->SetPoint2(event->pos());
    }
}

void MouseEvent_Line::MouseMoveEvent(QMouseEvent *event)
{
    MouseEventData_Line *data = static_cast<MouseEventData_Line*>(GetData());
    if(data!=nullptr)
    {
        data->SetPoint2(event->pos());
    }
}

void MouseEvent_Line::MouseReleaseEvent(QMouseEvent *event)
{
    MouseEventData_Line *data = static_cast<MouseEventData_Line*>(GetData());
    if(data!=nullptr)
        data->SetPoint2(event->pos());
}

void MouseEvent_Line::draw(QPainter *painter)
{
    if(painter==nullptr)
        return;

    MouseEvent::draw(painter);
    MouseEventData_Line *data = static_cast<MouseEventData_Line*>(GetData());
    if(data!=nullptr)
    {
        painter->drawLine(data->GetPoint1(),data->GetPoint2());
    }
}

MouseEvent_Rectangle::MouseEvent_Rectangle()
{
    InitializeData(new MouseEventData_Rectangle);
}

MouseEvent_Rectangle::~MouseEvent_Rectangle()
{

}

void MouseEvent_Rectangle::MousePressEvent(QMouseEvent *event)
{
    MouseEventData_Rectangle *data = static_cast<MouseEventData_Rectangle*>(GetData());
    if(data!=nullptr)
    {
        data->SetRect(QRect(event->pos(),event->pos()));
    }
}

void MouseEvent_Rectangle::MouseMoveEvent(QMouseEvent *event)
{
    MouseEventData_Rectangle *data = static_cast<MouseEventData_Rectangle*>(GetData());
    if(data!=nullptr)
    {
        QPoint topleft = data->GetRect().topLeft();
        QPoint bottomright = event->pos();
        data->SetRect(QRect(topleft,bottomright));
    }
}

void MouseEvent_Rectangle::MouseReleaseEvent(QMouseEvent *event)
{
    MouseEventData_Rectangle *data = static_cast<MouseEventData_Rectangle*>(GetData());
    if(data!=nullptr)
    {
        QPoint topleft = data->GetRect().topLeft();
        QPoint bottomright = event->pos();
        data->SetRect(QRect(topleft,bottomright));
    }
}

void MouseEvent_Rectangle::draw(QPainter *painter)
{
    MouseEventData_Rectangle *data = static_cast<MouseEventData_Rectangle*>(GetData());
    MouseEvent::draw(painter);
    if(data!=nullptr)
    {
        painter->drawRect(data->GetRect());
    }
}

MouseEvent_Ellipse::MouseEvent_Ellipse()
{

}

MouseEvent_Ellipse::~MouseEvent_Ellipse()
{

}

void MouseEvent_Ellipse::draw(QPainter *painter)
{
    MouseEventData_Rectangle *data = static_cast<MouseEventData_Rectangle*>(GetData());
    MouseEvent::draw(painter);
    if(data!=nullptr)
    {
        painter->drawEllipse(data->GetRect());
    }
}

MouseEvent_Pencil::MouseEvent_Pencil()
{
    InitializeData(new MouseEventData_Pencil);
}

MouseEvent_Pencil::~MouseEvent_Pencil()
{

}

void MouseEvent_Pencil::MousePressEvent(QMouseEvent *event)
{
    MouseEventData_Pencil *data = static_cast<MouseEventData_Pencil*>(GetData());
    if(data!=nullptr)
    {
        data->AddPoint(event->pos());
    }
}

void MouseEvent_Pencil::MouseMoveEvent(QMouseEvent *event)
{
    MouseEventData_Pencil *data = static_cast<MouseEventData_Pencil*>(GetData());
    if(data!=nullptr)
    {
        data->AddPoint(event->pos());
    }
}

void MouseEvent_Pencil::MouseReleaseEvent(QMouseEvent *event)
{
    MouseEventData_Pencil *data = static_cast<MouseEventData_Pencil*>(GetData());
    if(data!=nullptr)
    {
        data->AddPoint(event->pos());
    }
}

void MouseEvent_Pencil::draw(QPainter *painter)
{
    MouseEventData_Pencil *data = static_cast<MouseEventData_Pencil*>(GetData());
    if(data!=nullptr)
    {
        const QVector<QPoint>* points = data->GetPoints();
        if(points->isEmpty())
            return;
        QVector<QPoint>::const_iterator itor = points->begin();
        QPoint p = *itor;
        ++itor;
        for(;itor<points->end();++itor)
        {
            painter->drawLine(p,*itor);
            p = *itor;
        }
    }
}

MouseEvent_Polygon::MouseEvent_Polygon()
{
   InitializeData(new MouseEventData_Pencil);
}

MouseEvent_Polygon::~MouseEvent_Polygon()
{

}

void MouseEvent_Polygon::MousePressEvent(QMouseEvent *event)
{
    MouseEventData_Pencil *data = static_cast<MouseEventData_Pencil*>(GetData());
    if(data!=nullptr)
    {
        data->AddPoint(event->pos());
        data->SetCurrentPos(event->pos());
    }
}

void MouseEvent_Polygon::MouseMoveEvent(QMouseEvent *event)
{
    MouseEventData_Pencil *data = static_cast<MouseEventData_Pencil*>(GetData());
    if(data!=nullptr)
    {
        data->SetCurrentPos(event->pos());
    }

}

void MouseEvent_Polygon::MouseReleaseEvent(QMouseEvent *event)
{
    if(event->button()==Qt::RightButton)
    {
        MouseEventData_Pencil *data = static_cast<MouseEventData_Pencil*>(GetData());
        if(data!=nullptr)
        {
            if(!data->GetPoints()->isEmpty())
            {
                data->SetCurrentPos(data->GetPoints()->at(0));
                data->AddPoint(data->GetPoints()->at(0));
            }
        }
    }
}

void MouseEvent_Polygon::draw(QPainter *painter)
{
    MouseEvent_Pencil::draw(painter);
    MouseEventData_Pencil *data = static_cast<MouseEventData_Pencil*>(GetData());
    if(data!=nullptr)
    {
        const QVector<QPoint> *points = data->GetPoints();
        if(points->size()>0)
            painter->drawLine(points->at(points->size()-1),data->GetCurrentPos());
    }
}


