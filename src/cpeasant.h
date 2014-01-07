#ifndef CPEASANT_H
#define CPEASANT_H

#include "cunit.h"

class CPeasant;

class CPeasant : public CUnit
{
public:
    CPeasant();

    static void initializeGL();

    virtual void requestCommand(CommandObject command, float x, float y, CBaseObject *obj);
private:
    static CModel model;
    static AnimationList animList;

    StateMove stateMove;
    StateStay stateStay;
};

#endif // CPEASANT_H
