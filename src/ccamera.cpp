#include "ccamera.h"
#include "math.h"

#define PI 3.14159265f

CCamera::CCamera():
    rotate(0.0f)
{
    curr_h = 2.0f;
    max_h = 3.0f;
    min_h = 1.5f;

    min_va = 30.0f;
    max_va = 60.0f;

    updateAngle();
}

void CCamera::look()
{
    float h = landscape->getHeight(posX,posY);

    v[0] = cos(rotate * PI/180) * cos(curr_va * PI/180);
    v[1] = sin(rotate * PI/180) * cos(curr_va * PI/180);
    v[2] = -sin(curr_va * PI/180);

    e[0] = posX+v[0];
    e[1] = posY+v[1];
    e[2]= h+curr_h+v[2];

    gluLookAt(posX,posY,h+curr_h,e[0],e[1],e[2],0.0,0.0,1.0);
}

void CCamera::setLandscape(CLandscape *_landscape)
{
    landscape = _landscape;
}

float CCamera::getPositionX()
{
    return posX;
}

float CCamera::getPositionY()
{
    return posY;
}

float CCamera::getPositionZ()
{
    return landscape->getHeight(posX,posY) + curr_h;
}

void CCamera::keyPressEvent(QKeyEvent *k)
{
    if(k->key() == Qt::Key_Up)
    {
        posX+=cos(rotate * PI/180)*0.05f;
        posY+=sin(rotate * PI/180) * 0.05f;
    }
    if(k->key() == Qt::Key_Left)
    {
        posX+=cos((rotate+90.0f) * PI/180)*0.05f;
        posY+=sin((rotate+90.0f) * PI/180) * 0.05f;
    }
    if(k->key() == Qt::Key_Down)
    {
        posX+=cos((rotate+180.0f) * PI/180)*0.05f;
        posY+=sin((rotate+180.0f) * PI/180) * 0.05f;
    }
    if(k->key() == Qt::Key_Right)
    {
        posX+=cos((rotate+270.0f) * PI/180)*0.05f;
        posY+=sin((rotate+270.0f) * PI/180) * 0.05f;

    }

    if(k->key() == Qt::Key_Z) rotate += 1.25;
    if(k->key() == Qt::Key_X) rotate -= 1.25;
}

void CCamera::mouseMoveEvent(QMouseEvent *me)
{

}

void CCamera::wheelEvent(QWheelEvent *we)
{
    float new_h = curr_h-we->delta()*0.0005;
    if(new_h > max_h) curr_h = max_h; else
    if(new_h < min_h) curr_h = min_h; else

    curr_h=new_h;

    updateAngle();
}

void CCamera::updateAngle()
{
    curr_va = min_va + ((max_va-min_va)/(max_h-min_h))*(curr_h - min_h);
}
