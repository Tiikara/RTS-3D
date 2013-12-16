#include "cselection.h"

#include "cscene.h"

#include "math.h"

CSelection::CSelection():
    bUpdated(true)
{
}

void CSelection::setScene(CScene *scene)
{
    this->scene = scene;
}

void CSelection::update()
{

}

void CSelection::draw()
{

}

bool CSelection::isUpdated()
{
    if(bUpdated)
    {
        bUpdated = false;
        return true;
    }
    return false;
}

void CSelection::mousePressEvent(QMouseEvent *me)
{
    if(me->buttons() == Qt::RightButton)
    {
        countSelected = 0;
        bUpdated = true;
        return;
    }

    if(me->buttons() != Qt::LeftButton)
        return;

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

void CSelection::mouseReleaseEvent(QMouseEvent *)
{

}

void CSelection::mouseMoveEvent(QMouseEvent *)
{

}
