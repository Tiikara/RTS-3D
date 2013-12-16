#ifndef CPEASANT_H
#define CPEASANT_H

#include "canimationobject.h"

class CPeasant : public CAnimationObject
{
public:
    CPeasant();

    static void initializeGL();
private:
    static CModel model;
    static AnimationList animList;
};

#endif // CPEASANT_H
