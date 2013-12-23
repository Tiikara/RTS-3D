#ifndef CUNIT_H
#define CUNIT_H

#include "canimationobject.h"

struct IState
{
    virtual void start(){}
    virtual void update(){}
    virtual bool end(){return true;}
};

class CUnit : public CAnimationObject
{
public:
    CUnit(IdObject id, float radius, CModel *model, AnimationList *animations);

    void setState(IState *state);

    virtual void update();
private:
    IState *currState;
    IState *nextState;
};

#endif // CUNIT_H
