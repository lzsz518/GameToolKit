#ifndef SYSTEMDEFINE_H
#define SYSTEMDEFINE_H

#include <QVector>
#include <QImage>
#include <QColor>

enum DrawStatus{EMPTY,DRAWLINE,DRAWPENCIL,DRAWELLIPSE,DRAWRECTANGLE,DRAWPOLYGON};

using std::vector;
using std::pair;

struct MainWindowStatus
{
    DrawStatus CurrentDrawToolKit;
    QColor CurrentColor;
};

void FindRowPair(const QImage &mat, vector<pair<int,int>> &rowpairs);
void FindColumnPair(const QImage &mat, vector<pair<int,int>> &columnpairs);

QImage* CopyImageROI(const QRect &rect, const QImage &image);

#endif // SYSTEMDEFINE_H
