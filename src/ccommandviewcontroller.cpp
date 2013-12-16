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

void CCommandViewController::drawInterface()
{
    CWindow::begin2DMode();

    baseInterface.draw();

    CWindow::end2DMode();
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

void CCommandViewController::mouseMoveEvent(QMouseEvent *me)
{
    baseInterface.mouseMoveEvent(me->x(),QApplication::activeWindow()->height()-me->y(),me->buttons()==Qt::LeftButton? true : false,me->buttons()==Qt::RightButton? true : false);
}

void CCommandViewController::mousePressEvent(QMouseEvent *me)
{
    baseInterface.mousePressEvent(me->x(),QApplication::activeWindow()->height()-me->y(),me->buttons()==Qt::LeftButton? true : false,me->buttons()==Qt::RightButton? true : false);

    int count = selection->getCountSelected();
    CBaseObject **objects = selection->getSelectedObjects();

    for(int i=0;i<count;i++)
        objects[i]->requestCommand(CommandObject::Move);
}

void CCommandViewController::mouseReleaseEvent(QMouseEvent *me)
{
    baseInterface.mouseReleaseEvent(me->x(),QApplication::activeWindow()->height()-me->y(),me->buttons()==Qt::LeftButton? true : false,me->buttons()==Qt::RightButton? true : false);
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
