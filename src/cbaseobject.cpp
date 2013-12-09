#include "cbaseobject.h"

CBaseObject::CBaseObject()
{
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
}

void CBaseObject::setPosition(float x, float y, float z)
{
    pos[0] = x;
    pos[1] = y;
    pos[2] = z;
}
