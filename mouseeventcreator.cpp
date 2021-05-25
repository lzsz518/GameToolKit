#include "mouseevent.h"
#include "mouseeventcreator.h"

MouseEventCreator::MouseEventCreator()
{

}

MouseEventCreator::~MouseEventCreator()
{

}

MouseEvent* MouseEventCreator::CreateMouseEvent(const DrawStatus status)
{
    MouseEvent *result = nullptr;
    switch(status)
    {
    case DRAWLINE:
        result = new MouseEvent_Line;
        break;
    case DRAWRECTANGLE:
        result = new MouseEvent_Rectangle;
        break;
    case DRAWPENCIL:
        result = new MouseEvent_Pencil;
        break;
    case DRAWPOLYGON:
        result = new MouseEvent_Polygon;
        break;
    case DRAWELLIPSE:
        result = new MouseEvent_Ellipse;
        break;
    default:
        result = nullptr;
    }

    return result;
}
