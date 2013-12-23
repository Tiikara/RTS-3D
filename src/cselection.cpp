#include "cselection.h"

#include "cscene.h"

#include "math.h"

CSelection::CSelection():
    countSelected(0),
    bMousePressed(false),
    bGroupSelection(false),
    bCheckerUpdated(false),
    bUpdated(true)
{
}

void CSelection::setScene(CScene *scene)
{
    this->scene = scene;
}

void CSelection::update()
{
    if(bCheckerUpdated)
    {
        bUpdated = false;
        bCheckerUpdated = false;
    }
    else
    if(bUpdated)
    {
        bCheckerUpdated = true;
    }
}

void CSelection::draw2d()
{
    if(bGroupSelection)
    {
        glColor3f(0.0f,1.0f,0.0f);
        glLineWidth(2.0f);
        glBegin(GL_LINE_STRIP);
        glVertex2f(posStart[0],posStart[1]);
        glVertex2f(posEnd[0],posStart[1]);
        glVertex2f(posEnd[0],posEnd[1]);
        glVertex2f(posStart[0],posEnd[1]);
        glVertex2f(posStart[0],posStart[1]);
        glEnd();
        glColor3f(1.0f,1.0f,1.0f);
    }
}

bool CSelection::isUpdated()
{
    return bUpdated;
}

void CSelection::mousePressEvent(CMouseEvent *me)
{
    if(me->buttons() == Qt::RightButton)
    {
        countSelected = 0;
        bUpdated = true;
        return;
    }

    if(me->buttons() == Qt::LeftButton)
    {
        bMousePressed = true;
        posStart[0] = me->x();
        posStart[1] = me->y();
    }
}

void CSelection::mouseReleaseEvent(CMouseEvent *me)
{
    if(bMousePressed)
    {
        bMousePressed = false;

        if(bGroupSelection)
        {
            bGroupSelection = false;
            groupSelect(me);
        }
        else
        {
            singleSelect(me);
        }
    }
}

void CSelection::mouseMoveEvent(CMouseEvent *me)
{
    if(bMousePressed)
    {
        posEnd[0] = me->x();
        posEnd[1] = me->y();

        if(!bGroupSelection && (
                fabsf(posStart[0]-posEnd[0]) > 10.0f ||
                fabsf(posStart[1]-posEnd[1]) > 10.0f))
        {
            bGroupSelection = true;
        }
    }
}

void CSelection::singleSelect(CMouseEvent *me)
{
    vec3f posM;
    scene->getLandscape()->getIntersectPosition(me->x(),me->y(),posM);

    CBaseObject *objBuf[30];
    int countBuf=0;

    for(int w=-1;w<2;w++)
        for(int h=-1;h<2;h++)
        {
            Cell *curCell = scene->getCellFromPosition(posM[0],posM[1],w,h) ;
            int count = curCell->countObjects;
            CBaseObject **objects = curCell->objects;

            for(int i=0;i<count;i++)
            {
                objBuf[countBuf++] = objects[i];
           }
        }

    float *pos;
    float imp1,imp2;

    for(int i=0;i<countBuf;i++)
    {
        pos = objBuf[i]->getPosition();

        imp1 = posM[0]-pos[0];
        imp2 = posM[1]-pos[1];

        if(sqrt(imp1*imp1+imp2*imp2) < objBuf[i]->getRadiusObject())
        {
            selectedObjects[0] = objBuf[i];
            countSelected = 1;

            bUpdated = true;
        }
    }
}

void CSelection::groupSelect(CMouseEvent *)
{
    vec3f start;
    vec3f end;

    CLandscape *land = scene->getLandscape();

    land->getIntersectPosition(posStart[0],posStart[1],start);
    land->getIntersectPosition(posEnd[0],posEnd[1],end);

    for(int i=0;i<2;i++)
    {
        if(start[i] > end[i])
            std::swap(start[i],end[i]);
    }

    Cell *cells[50];
    int count;

    scene->getCellsFromRectangle(cells,&count,start[0],start[1],end[0],end[1]);

    bool first = true;
    for(int i=0;i<count;i++)
    {
        Cell *currCell = cells[i];
        for(int j=0;j<currCell->countObjects;j++)
        {
            float *posObj = currCell->objects[j]->getPosition();
            if(posObj[0] < end[0] && posObj[0] > start[0] && posObj[1] < end[1] && posObj[1] > start[1])
            {
                if(first)
                {
                    countSelected = 0;
                    first = false;
                }

                selectedObjects[countSelected++] = currCell->objects[j];
            }
        }

    }
}
