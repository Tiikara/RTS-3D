#ifndef CBASEOBJECT_H
#define CBASEOBJECT_H

typedef float vec3f[3];
#include "cmodel.h"

enum class IdObject : int
{
    Peasant
};

enum class CommandObject : int
{
    Attack,
    Defense,
    Move
};

class CBaseObject
{
public:
    CBaseObject();

    float *getPosition();
    float getRadiusObject();
    void setPosition(float *position);
    void setPosition(float x, float y, float z);

    IdObject getId(){ return id; }

    virtual void requestCommand(CommandObject){}

    virtual void draw(){}
    virtual void update(){}
protected:
    CModel *model;

    IdObject id;

    vec3f pos;
    float radius;
};

#endif // CBASEOBJECT_H
