#include "cpeasant.h"

#include "math.h"

#include "cscene.h"

CModel CPeasant::model;
AnimationList CPeasant::animList;

CPeasant::CPeasant():
    CUnit(IdObject::Peasant,0.35f,&CPeasant::model,&CPeasant::animList)
{
    stateStay._this = this;
    stateMove._this = this;
    stateMove.nextState = &stateStay;

    setState(&stateStay);
}

void CPeasant::initializeGL()
{
    CPeasant::model.initializeGL();
    CPeasant::model.loadFromFile("peasant.3d","form_front.tga");
    CPeasant::animList.loadAnimationDesc("peasant_animation_desc.txt");
}

void CPeasant::requestCommand(CommandObject command, float x, float y, CBaseObject *)
{
    switch(command)
    {
    case CommandObject::Move:
        stateMove.newPos[0] = x;
        stateMove.newPos[1] = y;
        setState(&stateMove);
        break;
    case CommandObject::Attack:
        break;
    case CommandObject::Defense:
        break;
    }
}
