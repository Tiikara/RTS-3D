#ifndef CUNIT_H
#define CUNIT_H

#include "canimationobject.h"

class CUnit;

struct IState
{
    virtual void start(){}
    virtual void update(){}
    virtual bool end(){return true;}
};

struct StateMove : public IState
{
    CUnit *_this;

    IState *nextState;

    virtual void start();
    virtual void update();

    float newPos[2];
};

struct StateStay : public IState
{
    CUnit *_this;

    virtual void start();
};

class CUnit : public CAnimationObject
{
public:
    CUnit(IdObject id, float radius, CModel *model, AnimationList *animations);

    void setState(IState *state);

    virtual void update();

    friend struct StateMove;
    friend struct StateStay;
private:
    IState *currState;
    IState *nextState;
};

#endif // CUNIT_H
