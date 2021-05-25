#ifndef QVCVMOUSEEVENT_H
#define QVCVMOUSEEVENT_H

#include <QPen>
#include <QWidget>



class MouseEventData
{
public:
    MouseEventData(){}
    ~MouseEventData(){}
};

class MouseEventData_Shape : public MouseEventData
{
   public:
    MouseEventData_Shape(){
        pen = nullptr;
    }
    ~MouseEventData_Shape(){
        if(pen!=nullptr)
            delete pen;
    }

    void SetPen(const QPen &p){
        if(pen==nullptr)
        {
            pen = new QPen(p);
        }
        else {
            *pen = QPen(p);
        }
    }
    void GetPen(QPen *p){
        if(pen!=nullptr)
        {
            if(p!=nullptr)
                delete p;

            p = new QPen;
            *p = *pen;
        }
        else
            p = nullptr;
    }

private:
    QPen *pen;
};

class MouseEventData_Line : public MouseEventData_Shape
{
public:
    MouseEventData_Line(){
        Point1 = QPoint(0,0);
        Point2 = QPoint(0,0);
    }
    ~MouseEventData_Line();
    void SetPoint1(const QPoint &p){
        Point1 = p;
    }
    void SetPoint2(const QPoint &p){
        Point2 = p;
    }
    QPoint GetPoint1(){
        return Point1;
    }
    QPoint GetPoint2(){
        return Point2;
    }
    void GetPoints(QPoint &p1,QPoint &p2){
        p1 = Point1;
        p2 = Point2;
    }
private:
    QPoint Point1;
    QPoint Point2;
};

class MouseEventData_Rectangle : public MouseEventData_Shape
{
public:
    MouseEventData_Rectangle(){
        rect = QRect(0,0,0,0);
    }
    ~MouseEventData_Rectangle();
    QRect GetRect(){
        return rect;
    }
    void SetRect(const QRect &r){
        rect = r;
    }
private:
        QRect rect;
};

class MouseEventData_Pencil :public MouseEventData_Shape
{
public:
    MouseEventData_Pencil(){

    }
    ~MouseEventData_Pencil(){

    }
    void AddPoint(const QPoint &p){
        points.append(p);
    }
    const QVector<QPoint>* GetPoints(){
        return &points;
    }
    QVector<QPoint>& GetPointsRef(){
        return points;
    }
    void SetCurrentPos(const QPoint &pos){
        currentPos = pos;
    }
    QPoint GetCurrentPos(){
        return currentPos;
    }
private:
    QPoint currentPos;
    QVector<QPoint> points;
};


class MouseEvent : public QWidget
{
    Q_OBJECT
public:
    explicit MouseEvent(QWidget *parent = nullptr);
    virtual ~MouseEvent();
signals:

public slots:

public:
    virtual void MousePressEvent(QMouseEvent *event)=0;
    virtual void MouseMoveEvent(QMouseEvent *event);
    virtual void MouseReleaseEvent(QMouseEvent *event);
    virtual void draw(QPainter *painter);

    virtual MouseEventData* GetData();
    void InitializeData(MouseEventData *d);
private:
    MouseEventData *mouse_data;
};

class MouseEvent_Line : public MouseEvent
{
    Q_OBJECT
public:
    MouseEvent_Line();
    virtual ~MouseEvent_Line();
public:
    virtual void MousePressEvent(QMouseEvent *event);
    virtual void MouseMoveEvent(QMouseEvent *event);
    virtual void MouseReleaseEvent(QMouseEvent *event);
    virtual void draw(QPainter *painter);
};

class MouseEvent_Rectangle : public MouseEvent
{
    Q_OBJECT
public:
    MouseEvent_Rectangle();
    virtual ~MouseEvent_Rectangle();
public:
    virtual void MousePressEvent(QMouseEvent *event);
    virtual void MouseMoveEvent(QMouseEvent *event);
    virtual void MouseReleaseEvent(QMouseEvent * event);
    virtual void draw(QPainter *painter);
};

class MouseEvent_Ellipse : public MouseEvent_Rectangle
{
    Q_OBJECT
public:
    MouseEvent_Ellipse();
    virtual ~MouseEvent_Ellipse();
public:
    virtual void draw(QPainter *painter);
};

class MouseEvent_Pencil : public MouseEvent
{
    Q_OBJECT
public:
    MouseEvent_Pencil();
    ~MouseEvent_Pencil();

public:
    virtual void MousePressEvent(QMouseEvent *event);
    virtual void MouseMoveEvent(QMouseEvent *event);
    virtual void MouseReleaseEvent(QMouseEvent * event);
    virtual void draw(QPainter *painter);
};

class MouseEvent_Polygon : public MouseEvent_Pencil
{
    Q_OBJECT
public:
    MouseEvent_Polygon();
    ~MouseEvent_Polygon();
public:
    virtual void MousePressEvent(QMouseEvent *event);
    virtual void MouseMoveEvent(QMouseEvent *event);
    virtual void MouseReleaseEvent(QMouseEvent * event);
    virtual void draw(QPainter *painter);
};


#endif // QVCVMOUSEEVENT_H
