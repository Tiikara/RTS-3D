#include "cunit.h"

CUnit::CUnit(IdObject _id, float _radius, CModel *_model, AnimationList *_animations):
    CAnimationObject(_id,_radius,_model,_animations),
    currState(NULL),
    nextState(NULL)
{
}

void CUnit::setState(IState *state)
{
    nextState = state;

    if(currState==NULL)
    {
        currState = nextState;
        currState->start();
    }

    if(currState==nextState)
        currState->start();
}

void CUnit::update()
{
    CAnimationObject::update();

    if(currState==NULL)
        return;

    if(currState!=nextState)
    {
        if(currState->end())
        {
            currState = nextState;
            currState->start();
        }
    }
    else
    {
       currState->update();
    }
}
