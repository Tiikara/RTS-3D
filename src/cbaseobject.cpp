#include "cbaseobject.h"

CBaseObject::CBaseObject(IdObject _id, float _radius, CModel *_model):
    id(_id),
    radius(_radius),
    model(_model)
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
}
