#include "ccommandviewcontroller.h"

CCommandViewController::CCommandViewController()
{
    connect(&butAttack, SIGNAL(clicked()), this, SLOT(commandAttack()));
    connect(&butDef, SIGNAL(clicked()), this, SLOT(commandDefense()));
}

void CCommandViewController::initializeGL()
{
    baseInterface.loadTexture("tex/int.png");
    butAttack.loadTextures("tex/butNormal.png","tex/butSelected.png","tex/butPressed.png");
    butDef.loadTextures("tex/butNormal.png","tex/butSelected.png","tex/butPressed.png");

    peasantIcon.loadTexture("tex/peasant.png");

    baseInterface.setPosition(10,10);
}

void CCommandViewController::setSelection(CSelection *selection)
{
    this->selection = selection;
}

void CCommandViewController::setLandscape(CLandscape *landscape)
{
    this->landscape = landscape;
}

void CCommandViewController::drawInterface()
{
    baseInterface.draw();
}

void CCommandViewController::updateInterface()
{
    if(selection->isUpdated())
    {
        baseInterface.clearAllChilds();

        if(selection->getCountSelected() == 0)
            return;

        switch(selection->getSelectedObjects()[0]->getId())
        {
        case IdObject::Peasant:
            baseInterface.addChild(&butAttack,300,30);
            baseInterface.addChild(&butDef,420,30);
            baseInterface.addChild(&peasantIcon,0,0);
            break;
        }
    }
}

void CCommandViewController::mouseMoveEvent(CMouseEvent *me)
{
    baseInterface.mouseMoveEvent(me->x(),me->y(),me->buttons()==Qt::LeftButton,me->buttons()==Qt::RightButton);
}

void CCommandViewController::mousePressEvent(CMouseEvent *me)
{
    float pos[3];

    if(me->buttons() != Qt::LeftButton)
        return;

    landscape->getIntersectPosition(me->x(),me->y(),pos);

    baseInterface.mousePressEvent(me->x(),me->y(),me->buttons()==Qt::LeftButton,me->buttons()==Qt::RightButton);

    int count = selection->getCountSelected();
    CBaseObject **objects = selection->getSelectedObjects();

    for(int i=0;i<count;i++)
        objects[i]->requestCommand(CommandObject::Move,pos[0],pos[1]);
}

void CCommandViewController::mouseReleaseEvent(CMouseEvent *me)
{
    baseInterface.mouseReleaseEvent(me->x(),me->y(),me->buttons()==Qt::LeftButton,me->buttons()==Qt::RightButton);
}

void CCommandViewController::mousePositionEvent(QPoint *p)
{
    baseInterface.mousePositionEvent(p->x(),p->y());
}

void CCommandViewController::commandDefense()
{
    int count = selection->getCountSelected();
    CBaseObject **objects = selection->getSelectedObjects();

    for(int i=0;i<count;i++)
        objects[i]->requestCommand(CommandObject::Defense);
}

void CCommandViewController::commandAttack()
{
    int count = selection->getCountSelected();
    CBaseObject **objects = selection->getSelectedObjects();

    for(int i=0;i<count;i++)
        objects[i]->requestCommand(CommandObject::Attack);
}
