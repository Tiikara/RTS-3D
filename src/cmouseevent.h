#ifndef CMOUSEEVENT_H
#define CMOUSEEVENT_H

#include <QMouseEvent>

class MainWindow;

class CMouseEvent
{
public:
    float x() const { return fx; }
    float y() const { return fy; }

    Qt::MouseButtons buttons() const { return button; }

    friend class MainWindow;
private:
    int fx;
    int fy;

    Qt::MouseButtons button;
};

#endif // CMOUSEEVENT_H
