#ifndef SYSTEMDEFINE_H
#define SYSTEMDEFINE_H

#include <QVector>
#include <QImage>
#include <QColor>

#define SPRITE_TYPE Qt::UserRole +1

#define SPRITE_ROOT 1
#define SPRITE_ITEM 2
#define SPRITE_POS Qt::UserRole +3
#define SPRITE_IMG Qt::UserRole +4

enum DrawStatus{EMPTY,DRAWLINE,DRAWPENCIL,DRAWELLIPSE,DRAWRECTANGLE,DRAWPOLYGON};

using std::vector;
using std::pair;

const QString SpriteMimeDataType = QStringLiteral("Image/Sprite");

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
