#include "cbaseobject.h"

CBaseObject::CBaseObject(IdObject _id, float _radius, CModel *_model):
    model(_model),
    id(_id),
    radius(_radius)

{
    for(int i=0;i<3;i++)
        pos[0] = 0.0f;
}

float *CBaseObject::getPosition()
{
    return pos;
}

float CBaseObject::getRadiusObject()
{
    return radius;
}

void CBaseObject::setPosition(float *position)
{
    pos[0] = position[0];
    pos[1] = position[1];
    pos[2] = position[2];

    model->setPosition(position);
}

void CBaseObject::setPosition(float x, float y, float z)
{
    pos[0] = x;
    pos[1] = y;
    pos[2] = z;
    model->setPosition(x,y,z);
}

void CBaseObject::setRotate(float angle)
{
    rotateAngle = angle;
    model->setRotateParam(angle,0.0f,0.0f,1.0f);
}

void CBaseObject::requestCommand(CommandObject,float,float, CBaseObject*)
{}
