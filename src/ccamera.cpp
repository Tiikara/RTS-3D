#include "ccamera.h"
#include "math.h"

#define PI 3.14159265f

enum CAM_KEYS {Key_Up, Key_Down, Key_Left, Key_Right};

CCamera::CCamera():
    max_va(60.0f),
    min_va(30.0f),
    max_h(30.0f),
    min_h(2.5f),
    curr_h(2.0f),
    rotate(0.0f),
    moveSpeed(0.3f),
    speedScroll(0.005f),
    old_mx(-100)
{
    for(int i=0;i<3;i++)
        pos[i] = 0.0f;

    for(int i=0;i<4;i++)
        pressedKeys[i] = false;

    updateAngle();
}

void CCamera::look()
{
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
    if(k->key() == Qt::Key_Up) pressedKeys[Key_Up] = true;
    if(k->key() == Qt::Key_Left) pressedKeys[Key_Left] = true;
    if(k->key() == Qt::Key_Down) pressedKeys[Key_Down] = true;
    if(k->key() == Qt::Key_Right) pressedKeys[Key_Right] = true;

    if(k->key() == Qt::Key_Z) rotate += 1.25;
    if(k->key() == Qt::Key_X) rotate -= 1.25;
}

void CCamera::keyReleaseEvent(QKeyEvent *k)
{
    if(k->key() == Qt::Key_Up) pressedKeys[Key_Up] = false;
    if(k->key() == Qt::Key_Left) pressedKeys[Key_Left] = false;
    if(k->key() == Qt::Key_Down) pressedKeys[Key_Down] = false;
    if(k->key() == Qt::Key_Right) pressedKeys[Key_Right] = false;
}

void CCamera::mouseMoveEvent(QMouseEvent *me)
{
    int xp=me->x();
    if (old_mx!=-100)
    {
        if (me->buttons()==Qt::MiddleButton)
            rotate+=((xp-old_mx)/QApplication::activeWindow()->width())*90.0f;
    }
    old_mx=xp;

}

void CCamera::wheelEvent(QWheelEvent *we)
{
    float new_h = curr_h-we->delta()*speedScroll;
    if(new_h > max_h) curr_h = max_h; else
    if(new_h < min_h) curr_h = min_h; else

    curr_h=new_h;

    updateAngle();
}

void CCamera::updateAngle()
{
    curr_va = min_va + ((max_va-min_va)/(max_h-min_h))*(curr_h - min_h);
}

void CCamera::update()
{
    if(pressedKeys[Key_Up])
    {
        pos[0]+=cos(rotate * PI/180)*moveSpeed;
        pos[1]+=sin(rotate * PI/180)*moveSpeed;
    }
    if(pressedKeys[Key_Left])
    {
        pos[0]+=cos((rotate+90.0f) * PI/180)*moveSpeed;
        pos[1]+=sin((rotate+90.0f) * PI/180)*moveSpeed;
    }
    if(pressedKeys[Key_Down])
    {
        pos[0]+=cos((rotate+180.0f) * PI/180)*moveSpeed;
        pos[1]+=sin((rotate+180.0f) * PI/180)*moveSpeed;
    }
    if(pressedKeys[Key_Right])
    {
        pos[0]+=cos((rotate+270.0f) * PI/180)*moveSpeed;
        pos[1]+=sin((rotate+270.0f) * PI/180)*moveSpeed;
    }

    pos[2] = landscape->getHeight(pos[0],pos[1]) + curr_h;

    vdir[0] = pos[0] + cos(rotate * PI/180) * cos(curr_va * PI/180);
    vdir[1] = pos[1] + sin(rotate * PI/180) * cos(curr_va * PI/180);
    vdir[2] = pos[2] - sin(curr_va * PI/180);
}
