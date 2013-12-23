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
    commandVController.setLandscape(&landscape);
}

vec3f posPoint = {0};
CPeasant *peasant;

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

    CPeasant::initializeGL();

    peasant = new CPeasant;
    objects[0] = peasant;
    peasant->setPosition(0,0,1);
    countObjects=1;
}

void CScene::draw()
{
    camera.look();
    landscape.draw();

    for(int i=0;i<countObjects;i++)
        objects[i]->draw();

    for(int i=0;i<countTrees;i++)
        trees[i]->draw();

    glPointSize(30);
    glBegin(GL_POINTS);
    glVertex3fv(posPoint);
    glEnd();

    CTexture::begin2DMode();

    selection.draw2d();
    commandVController.drawInterface();

    CTexture::end2DMode();
}

void CScene::update()
{
    updateCells();

    for(int i=0;i<countObjects;i++)
        objects[i]->update();

    camera.update();
    landscape.update();

    commandVController.updateInterface();
}

Cell *CScene::getCellFromPosition(float x, float y, int offsetRow, int offsetColumn)
{
    return &cells[ (countCellsH * int((x+sizeWidth/2.0f)/sizeCellW) + offsetRow) + int((y+sizeHeight/2.0f)/sizeCellH) + offsetColumn ];
}

void CScene::getCellsFromRectangle(Cell **buf, int *count, float startX, float startY, float endX, float endY)
{
    int startW = int((startX+sizeWidth/2.0f)/sizeCellW) - 1;
    int startH = int((startY+sizeHeight/2.0f)/sizeCellH) - 1;

    int endW = int((endX+sizeWidth/2.0f)/sizeCellW) + 1;
    int endH = int((endY+sizeHeight/2.0f)/sizeCellH) + 1;

    *count = 0;

    for(int i=startW;i<endW;i++)
        for(int j=startH;j<endH;j++)
        {
            buf[(*count)++] = &cells[ countCellsH * i + j ];
        }
}

void CScene::keyPressEvent(QKeyEvent *k)
{
    camera.keyPressEvent(k);

    if(k->key() == Qt::Key_1)
        peasant->setAnimation("ходьба");
    if(k->key() == Qt::Key_2)
        peasant->setAnimation("дрова");
    if(k->key() == Qt::Key_3)
        peasant->setAnimation("атака");
    if(k->key() == Qt::Key_4)
        peasant->setAnimation("базовое стояние");
    if(k->key() == Qt::Key_5)
        peasant->setAnimation("стояние 2");
    if(k->key() == Qt::Key_6)
        peasant->setAnimation("стояние 3");
    if(k->key() == Qt::Key_7)
        peasant->setAnimation("критический ущерб");
    if(k->key() == Qt::Key_8)
        peasant->setAnimation("смерть");
}

void CScene::keyReleaseEvent(QKeyEvent *k)
{
    camera.keyReleaseEvent(k);
}

void CScene::mouseMoveEvent(CMouseEvent *me)
{
    camera.mouseMoveEvent(me);

    selection.mouseMoveEvent(me);

    commandVController.mouseMoveEvent(me);
}

void CScene::mousePressEvent(CMouseEvent *me)
{
    commandVController.mousePressEvent(me);

    if(me->buttons() == Qt::LeftButton)
    {
        landscape.getIntersectPosition(me->x(),me->y(),posPoint);
    }

    selection.mousePressEvent(me);
}

void CScene::mouseReleaseEvent(CMouseEvent *me)
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
