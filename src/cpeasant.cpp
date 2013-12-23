#include "cpeasant.h"

#include "math.h"

#define PI 3.14159265f

CModel CPeasant::model;
AnimationList CPeasant::animList;

CPeasant::CPeasant():
    CUnit(IdObject::Peasant,1.0f,&CPeasant::model,&CPeasant::animList)
{
    stateStay.peasant = this;
    stateMove.peasant = this;

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
        newPos[0] = x;
        newPos[1] = y;
        setState(&stateMove);
        break;
    case CommandObject::Attack:
        break;
    case CommandObject::Defense:
        break;
    }
}

void StateMove::start()
{
    peasant->setAnimation("ходьба");

    float transPos[2] = { peasant->newPos[0] - peasant->pos[0] , peasant->newPos[1] - peasant->pos[1] };

    float newRotate = atan2f(transPos[0],transPos[1]) * 180.0f/PI;

    float length = 1.0f/sqrtf(transPos[0]*transPos[0]+transPos[1]*transPos[1]);
    peasant->dirMove[0] = length * transPos[0] * 1.0f;
    peasant->dirMove[1] = length * transPos[1] * 1.0f;

    if(newRotate>0)
        newRotate=180.0f-newRotate+180.0f;
    else
        newRotate =(-newRotate);

    peasant->newRotate = newRotate;
}

void StateMove::update()
{
    if(peasant->rotateAngle != peasant->newRotate)
    {
        if(peasant->rotateAngle<peasant->newRotate)
        {
            peasant->rotateAngle += 15.1f;
            if(peasant->rotateAngle>peasant->newRotate)
                peasant->rotateAngle = peasant->newRotate;
        }
        else
        {
            peasant->rotateAngle -= 15.1f;
            if(peasant->rotateAngle<peasant->newRotate)
                peasant->rotateAngle = peasant->newRotate;
        }

       peasant->setRotate(peasant->rotateAngle);
    }

    peasant->pos[0]+=peasant->dirMove[0];
    peasant->pos[1]+=peasant->dirMove[1];

    if(fabsf(peasant->pos[0]-peasant->newPos[0])<=fabsf(peasant->dirMove[0]) &&
       fabsf(peasant->pos[1]-peasant->newPos[1])<=fabsf(peasant->dirMove[1]))
    {
        peasant->setState(&peasant->stateStay);
        peasant->pos[0]=peasant->newPos[0];
        peasant->pos[1]=peasant->newPos[0];
    }

    peasant->setPosition(peasant->pos);
}

void StateStay::start()
{
    peasant->setAnimation("базовое стояние");
}
