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

struct SpriteRectangle
{
    QRect image_area;
    vector<pair<int,int>> rowpairs;
    vector<vector<pair<int,int>>> columnpairs;
};

void FindRowPair(const QImage &mat, vector<pair<int,int>> &rowpairs);
void FindColumnPair(const QImage &mat, vector<pair<int,int>> &columnpairs);

QImage* CopyImageROI(const QRect &rect, const QImage &image);

SpriteRectangle* FindRowAndColumnPair(const QImage &img, const QRect &rect);

#endif // SYSTEMDEFINE_H
