#ifndef CSELECTION_H
#define CSELECTION_H

#include "cbaseobject.h"
#include "clandscape.h"

class CScene;

class CSelection
{
public:
    CSelection();

    void setScene(CScene *scene);

    void update();
    void draw();

    void mousePressEvent(QMouseEvent *me);
    void mouseReleaseEvent(QMouseEvent *me);
    void mouseMoveEvent(QMouseEvent *me);
private:
    CScene *scene;

    int countSelected;
    CBaseObject *selectedObjects[20];
};

#endif // CSELECTION_H
