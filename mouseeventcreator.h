#ifndef QVCVMOUSEEVENTCREATOR_H
#define QVCVMOUSEEVENTCREATOR_H

#include "systemdefine.h"

class MouseEvent;

class MouseEventCreator
{
public:
    MouseEventCreator();
    ~MouseEventCreator();
    static MouseEvent* CreateMouseEvent(const DrawStatus status);
};

#endif // QVCVMOUSEEVENTCREATOR_H
