#ifndef CPEASANT_H
#define CPEASANT_H

#include "cunit.h"

class CPeasant;

struct StateMove : public IState
{
    CPeasant *peasant;

    virtual void start();
    virtual void update();
};

struct StateStay : public IState
{
    CPeasant *peasant;

    virtual void start();
};

class CPeasant : public CUnit
{
public:
    CPeasant();

    static void initializeGL();

    virtual void requestCommand(CommandObject command, float x, float y, CBaseObject *obj);

    friend struct StateMove;
    friend struct StateStay;
private:
    static CModel model;
    static AnimationList animList;

    int moveStateId;
    int stayStateId;

    StateMove stateMove;
    StateStay stateStay;

    float newPos[2];
    float newRotate;
    float dirMove[2];
};

#endif // CPEASANT_H
