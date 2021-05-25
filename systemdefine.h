#ifndef SYSTEMDEFINE_H
#define SYSTEMDEFINE_H

#include <QColor>

enum DrawStatus{EMPTY,DRAWLINE,DRAWPENCIL,DRAWELLIPSE,DRAWRECTANGLE,DRAWPOLYGON};

struct MainWindowStatus
{
    DrawStatus CurrentDrawToolKit;
    QColor CurrentColor;
};
#endif // SYSTEMDEFINE_H
