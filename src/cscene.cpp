#include "cscene.h"

#include "generateforest.h"

CScene::CScene():
    countTrees(0),
    countObjects(0),
    countObjectsBuffer(0),
    countCellsW(100),
    countCellsH(100),
    sizeWidth(5000),
    sizeHeight(5000)
{
    camera.setLandscape(&landscape);
    landscape.setCamera(&camera);

    sizeCellW = sizeWidth/countCellsW;
    sizeCellH = sizeHeight/countCellsH;

    cells = new Cell[countCellsW * countCellsH];
    objects = new CBaseObject*[1000000];
    objectsBuffer = new CBaseObject*[1000000];
    trees = new CTree*[1000000];

    selection.setScene(this);
    commandVController.setSelection(&selection);
}

vec3f posPoint = {0};

CScene::~CScene()
{
    delete[] cells;
    delete[] objects;
}

void CScene::initializeGL()
{
    landscape.loadSettings("landscape.ini");
    landscape.initializeGL();

    CTree::initializeGLModel();
    generateForests(trees,&countTrees,&landscape);

    commandVController.initializeGL();
}

void CScene::draw()
{
    camera.look();
    landscape.draw();

    for(int i=0;i<countTrees;i++)
        trees[i]->draw();

    glPointSize(30);
    glBegin(GL_POINTS);
    glVertex3fv(posPoint);
    glEnd();

    commandVController.drawInterface();
}

void CScene::update()
{
    updateCells();

    camera.update();
    landscape.update();

    commandVController.updateInterface();
}

Cell *CScene::getCellFromPosition(float x, float y)
{
    return &cells[ countCellsH * int((x+sizeWidth/2.0f)/sizeCellW) + int((y+sizeHeight/2.0f)/sizeCellH) ];
}

void CScene::keyPressEvent(QKeyEvent *k)
{
    camera.keyPressEvent(k);
}

void CScene::keyReleaseEvent(QKeyEvent *k)
{
    camera.keyReleaseEvent(k);
}

void CScene::mouseMoveEvent(QMouseEvent *me)
{
    camera.mouseMoveEvent(me);

    selection.mouseMoveEvent(me);

    commandVController.mouseMoveEvent(me);
}

void CScene::mousePressEvent(QMouseEvent *me)
{
    commandVController.mousePressEvent(me);

    if(me->button() == Qt::LeftButton)
    {
        landscape.getIntersectPosition(me->x(),me->y(),posPoint);
    }

    selection.mousePressEvent(me);
}

void CScene::mouseReleaseEvent(QMouseEvent *me)
{
    selection.mouseReleaseEvent(me);

    commandVController.mouseReleaseEvent(me);
}

void CScene::mousePositionEvent(QPoint *p)
{
    commandVController.mousePositionEvent(p);
}

void CScene::wheelEvent(QWheelEvent *we)
{
    camera.wheelEvent(we);
}

void CScene::updateCells()
{
    float *pos;
    Cell *curCell;

    memset(cells,0,sizeof(Cell)*countCellsW*countCellsH);

    countObjectsBuffer = 0;

    for(int i=0;i<countObjects;i++)
    {
        pos = objects[i]->getPosition();

        curCell = getCellFromPosition(pos[0],pos[1]);

        curCell->countObjects++;
    }

    for(int i=0;i<countObjects;i++)
    {
        pos = objects[i]->getPosition();

        curCell = getCellFromPosition(pos[0],pos[1]) ;

        if(curCell->objects==NULL)
        {
            curCell->objects = &objectsBuffer[countObjectsBuffer];
            countObjectsBuffer+=curCell->countObjects;
            curCell->countObjects=0;
        }

        curCell->objects[curCell->countObjects++] = objects[i];
    }
}
