#include "cbaseobject.h"

#define PI 3.14159265f

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
}

void CBaseObject::setPosition(float x, float y, float z)
{
    pos[0] = x;
    pos[1] = y;
    pos[2] = z;
}

void CBaseObject::setRotate(float angle)
{
    rotateAngle = angle;
    model->setRotateParam(angle,0.0f,0.0f,1.0f);
}

void CBaseObject::requestCommand(CommandObject,float,float, CBaseObject*)
{}

void CBaseObject::draw()
{
}

void CBaseObject::updateSmoothRotateTo(float x, float y, float speedRotate)
{
    float newRotate = atan2f(x - pos[0],y - pos[1]) * 180.0f/PI;

    if(newRotate>0)
        newRotate=360.0f-newRotate;
    else
        newRotate = -newRotate;

    if(rotateAngle != newRotate)
    {
        if(rotateAngle<newRotate)
        {
            rotateAngle += speedRotate;
            if(rotateAngle>newRotate)
                rotateAngle = newRotate;
        }
        else
        {
            rotateAngle -= speedRotate;
            if(rotateAngle<newRotate)
                rotateAngle = newRotate;
        }
    }
}
