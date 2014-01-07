#ifndef CSELECTION_H
#define CSELECTION_H

#include <QtOpenGL>

#include "cbaseobject.h"
#include "clandscape.h"

#include "cmouseevent.h"

class CScene;

class CSelection
{
public:
    CSelection();

    void initGL();

    int getCountSelected() { return countSelected; }
    CBaseObject **getSelectedObjects() { return selectedObjects; }

    void setScene(CScene *scene);

    void update();

    void draw();
    void draw2d();

    bool isUpdated();

    void mousePressEvent(CMouseEvent *me);
    void mouseReleaseEvent(CMouseEvent *me);
    void mouseMoveEvent(CMouseEvent *me);
private:
    void singleSelect(CMouseEvent *me);
    void groupSelect(CMouseEvent *me);

    CScene *scene;

    GLuint idTex;

    int countSelected;
    CBaseObject *selectedObjects[1000];

    bool bMousePressed;
    bool bGroupSelection;

    float posStart[2];
    float posEnd[2];

    bool bCheckerUpdated;
    bool bUpdated;
};

#endif // CSELECTION_H
