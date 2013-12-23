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
    CBaseObject(IdObject id, float radius, CModel *model);

    float *getPosition();
    float getRadiusObject();
    void setPosition(float *position);
    void setPosition(float x, float y, float z);
    void setRotate(float angle);
    float getRotateAngle() { return rotateAngle; }

    IdObject getId(){ return id; }

    virtual void requestCommand(CommandObject command,float x = 0, float y = 0,CBaseObject *obj = NULL);

    virtual void draw(){}
    virtual void update(){}
protected:
    CModel *model;

    IdObject id;

    vec3f pos;
    float radius;
    float rotateAngle;
};

#endif // CBASEOBJECT_H