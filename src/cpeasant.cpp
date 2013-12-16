#include "cpeasant.h"

CModel CPeasant::model;
AnimationList CPeasant::animList;

CPeasant::CPeasant():
    CAnimationObject(IdObject::Peasant,1.0f,&CPeasant::model,&CPeasant::animList)
{
}

void CPeasant::initializeGL()
{
    CPeasant::model.initializeGL();
    CPeasant::model.loadFromFile("peasant.3d","form_front.tga");
    CPeasant::animList.loadAnimationDesc("peasant_animation_desc.txt");
}

