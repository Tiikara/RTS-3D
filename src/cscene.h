#ifndef CSCENE_H
#define CSCENE_H

#include "ccamera.h"
#include "clandscape.h"
#include "ctree.h"
#include "cbaseobject.h"
#include "cselection.h"
#include "ccommandviewcontroller.h"
#include "ccursorcontroller.h"

#include "cpeasant.h"

#include "cmouseevent.h"

struct Cell
{
    int countObjects;
    CBaseObject **objects;
};

struct CellTree
{
    int countTrees;
    CTree **objects;
};

class CScene
{
public:
    CScene();
    ~CScene();

    void initializeGL();

    void draw();
    void update();

    static CScene *sharedScene();

    Cell *getCellFromPosition(float x, float y, int offsetRow = 0, int offsetColumn = 0);
    CellTree *getCellTreeFromPosition(float x, float y, int offsetRow = 0, int offsetColumn = 0);

    void getCellsFromRectangle(Cell **buf, int *count, float startX, float startY, float endX, float endY);

    CLandscape *getLandscape() { return &landscape; }
    CSelection *getSelection() { return &selection; }

    bool isFreeRegion(float x, float y, float radius, CBaseObject *expObj);

    void keyPressEvent(QKeyEvent *k);
    void keyReleaseEvent(QKeyEvent *k);
    void mouseMoveEvent(CMouseEvent *me);
    void mousePressEvent(CMouseEvent *me);
    void mouseReleaseEvent(CMouseEvent *me);
    void mousePositionEvent(QPoint *p);
    void wheelEvent(QWheelEvent *we);
private:
    void updateCells();
    void updateCellsTree();

    CLandscape landscape;
    CCamera camera;

    CSelection selection;

    CCommandViewController commandVController;
    CCursorController cursorController;

    CTree **trees;
    int countTrees;

    CTree **treesBuffer;
    int countTreesBuffer;

    CBaseObject **objects;
    int countObjects;

    CBaseObject **objectsBuffer;
    int countObjectsBuffer;

    CellTree *cellsTree;

    Cell *cells;
    int countCellsW;
    int countCellsH;

    float sizeCellW;
    float sizeCellH;

    float sizeWidth;
    float sizeHeight;
};

#endif // CSCENE_H
