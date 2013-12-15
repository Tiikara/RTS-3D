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
    vec3f posM;
    float *pos;
    scene->getLandscape()->getIntersectPosition(me->x(),QApplication::activeWindow()->height()-me->y(),posM);

    for(int w=-1;w<2;w++)
        for(int h=-1;h<2;h++)
        {
            Cell *curCell = scene->getCellFromPosition(posM[0],posM[1]) ;
            int count = curCell->countObjects;
            CBaseObject **objects = curCell->objects;

            float imp1,imp2;
            for(int i=0;i<count;i++)
            {
                pos = objects[i]->getPosition();

                imp1 = me->x()-pos[0];
                imp2 = me->y()-pos[1];

                if(sqrt(imp1*imp1+imp2*imp2) < objects[i]->getRadiusObject())
                {
                    selectedObjects[0] = objects[i];
                    countSelected = 1;

                    bUpdated = true;
                }
            }
        }
}

void CSelection::mouseReleaseEvent(QMouseEvent *)
{

}

void CSelection::mouseMoveEvent(QMouseEvent *)
{

}
