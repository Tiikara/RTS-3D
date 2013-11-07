#include "ccamera.h"
#include "math.h"

#define PI 3.14159265f

CCamera::CCamera():
    max_va(60.0f),
    min_va(30.0f),
    max_h(3.0f),
    min_h(1.5f),
    curr_h(2.0f),
    rotate(0.0f)
{
    pos[0] = 0.0f;
    pos[1] = 0.0f;
    pos[2] = 0.0f;
    updateAngle();
}

void CCamera::look()
{
    pos[2] = landscape->getHeight(pos[0],pos[1]) + curr_h;

    vdir[0] = pos[0] + cos(rotate * PI/180) * cos(curr_va * PI/180);
    vdir[1] = pos[1] + sin(rotate * PI/180) * cos(curr_va * PI/180);
    vdir[2] = pos[2] - sin(curr_va * PI/180);

    gluLookAt(pos[0],pos[1],pos[2],vdir[0],vdir[1],vdir[2],0.0,0.0,1.0);
}

void CCamera::setLandscape(CLandscape *_landscape)
{
    landscape = _landscape;
}

pvec3f CCamera::getPosition()
{
    return pos;
}

void CCamera::keyPressEvent(QKeyEvent *k)
{
    if(k->key() == Qt::Key_Up)
    {
        pos[0]+=cos(rotate * PI/180)*0.05f;
        pos[1]+=sin(rotate * PI/180) * 0.05f;
    }
    if(k->key() == Qt::Key_Left)
    {
        pos[0]+=cos((rotate+90.0f) * PI/180)*0.05f;
        pos[1]+=sin((rotate+90.0f) * PI/180) * 0.05f;
    }
    if(k->key() == Qt::Key_Down)
    {
        pos[0]+=cos((rotate+180.0f) * PI/180)*0.05f;
        pos[1]+=sin((rotate+180.0f) * PI/180) * 0.05f;
    }
    if(k->key() == Qt::Key_Right)
    {
        pos[0]+=cos((rotate+270.0f) * PI/180)*0.05f;
        pos[1]+=sin((rotate+270.0f) * PI/180) * 0.05f;

    }

    if(k->key() == Qt::Key_Z) rotate += 1.25;
    if(k->key() == Qt::Key_X) rotate -= 1.25;
}

void CCamera::mouseMoveEvent(QMouseEvent *)
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
