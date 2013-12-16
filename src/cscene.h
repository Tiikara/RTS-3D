#ifndef CSCENE_H
#define CSCENE_H

#include "ccamera.h"
#include "clandscape.h"
#include "ctree.h"
#include "cbaseobject.h"
#include "cselection.h"
#include "ccommandviewcontroller.h"

#include "cpeasant.h"

struct Cell
{
    int countObjects;
    CBaseObject **objects;
};

class CScene
{
public:
    CScene();
    ~CScene();

    void initializeGL();

    void draw();
    void update();

    Cell *getCellFromPosition(float x, float y, int offsetRow = 0, int offsetColumn = 0);
    CLandscape *getLandscape() { return &landscape; }

    void keyPressEvent(QKeyEvent *k);
    void keyReleaseEvent(QKeyEvent *k);
    void mouseMoveEvent(QMouseEvent *me);
    void mousePressEvent(QMouseEvent *me);
    void mouseReleaseEvent(QMouseEvent *me);
    void mousePositionEvent(QPoint *p);
    void wheelEvent(QWheelEvent *we);
private:
    void updateCells();

    CLandscape landscape;
    CCamera camera;

    CSelection selection;

    CCommandViewController commandVController;

    CTree **trees;
    int countTrees;

    CBaseObject **objects;
    int countObjects;

    CBaseObject **objectsBuffer;
    int countObjectsBuffer;

    Cell *cells;
    int countCellsW;
    int countCellsH;

    float sizeCellW;
    float sizeCellH;

    float sizeWidth;
    float sizeHeight;
};

#endif // CSCENE_H
