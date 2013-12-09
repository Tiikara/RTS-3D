#include "cwindow.h"

#include <QApplication>
#include <QGLShaderProgram>
#include <QGLWidget>
#include <GL/glu.h>

CWindow::CWindow()
{
}

void CWindow::setPosition(float x, float y)
{
    pos[0] = x;
    pos[1] = y;
}

float *CWindow::getPosition()
{
    return pos;
}

void CWindow::setWindowSize(float _width, float _height)
{
    width = _width;
    height = _height;
}

void CWindow::getWindowSize(float *width, float *height)
{
    *width=this->width;
    *height=this->height;
}

void CWindow::setPriority(int priority)
{
    this->priority = priority;
}

void CWindow::transformCoordToLocal(float *mouseX, float *mouseY)
{
    (*mouseX)-=pos[0];
    (*mouseY)-=pos[1];
}

bool CWindow::notInWindowMouse(float mouseX, float mouseY)
{
    if(mouseX < 0 || mouseY < 0 || mouseX > width || mouseY > height)
        return true;
    return false;
}

void CWindow::draw()
{
    glPushMatrix();
    glTranslatef(pos[0], pos[1],0.0f);

    for(int i=0;i<childs.size();i++)
    {
        childs[i]->draw();
    }

    glPopMatrix();
}

void CWindow::update()
{
    for(int i=0;i<childs.size();i++)
    {
        childs[i]->update();
    }
}

bool CWindow::mousePressEvent(float mouseX, float mouseY, bool leftMouse, bool rightMouse)
{
    transformCoordToLocal(&mouseX,&mouseY);

    if(notInWindowMouse(mouseX,mouseY))
        return false;

    return pressEvent(mouseX,mouseY,leftMouse,rightMouse);
}

bool CWindow::mouseReleaseEvent(float mouseX, float mouseY, bool leftMouse, bool rightMouse)
{
    transformCoordToLocal(&mouseX,&mouseY);

    if(notInWindowMouse(mouseX,mouseY))
        return false;

    return releaseEvent(mouseX,mouseY,leftMouse,rightMouse);
}

bool CWindow::mouseMoveEvent(float mouseX, float mouseY, bool leftMouse, bool rightMouse)
{
    transformCoordToLocal(&mouseX,&mouseY);

    if(notInWindowMouse(mouseX,mouseY))
        return false;

    return moveEvent(mouseX,mouseY,leftMouse,rightMouse);
}

bool CWindow::mousePositionEvent(float mouseX, float mouseY)
{
    transformCoordToLocal(&mouseX,&mouseY);

    if(notInWindowMouse(mouseX,mouseY))
        return false;

    return positionEvent(mouseX,mouseY);
}

void CWindow::begin2DMode()
{
    glDisable(GL_DEPTH_TEST);

    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    gluOrtho2D( 0,QApplication::activeWindow()->width(),0,QApplication::activeWindow()->height());
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void CWindow::end2DMode()
{
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);

    glEnable(GL_DEPTH_TEST);
}

bool CWindow::pressEvent(float mouseX, float mouseY, bool leftMouse, bool rightMouse)
{
    for(int i=0;i<childs.size();i++)
    {
        if(childs[i]->mousePressEvent(mouseX,mouseY,leftMouse,rightMouse))
            return true;
    }
    return false;
}

bool CWindow::releaseEvent(float mouseX, float mouseY, bool leftMouse, bool rightMouse)
{
    for(int i=0;i<childs.size();i++)
    {
        if(childs[i]->mouseReleaseEvent(mouseX,mouseY,leftMouse,rightMouse))
            return true;
    }
    return false;
}

bool CWindow::moveEvent(float mouseX, float mouseY, bool leftMouse, bool rightMouse)
{
    for(int i=0;i<childs.size();i++)
    {
        if(childs[i]->mouseMoveEvent(mouseX,mouseY,leftMouse,rightMouse))
            return true;
    }
    return false;
}

bool CWindow::positionEvent(float mouseX, float mouseY)
{
    for(int i=0;i<childs.size();i++)
    {
        if(childs[i]->mousePositionEvent(mouseX,mouseY))
            return true;
    }
    return false;
}

void CWindow::addChild(CWindow *window, float posX, float posY, int priority)
{
    window->setPosition(posX, posY);
    window->setPriority(priority);

    QList<CWindow*>::iterator i=childs.begin();

    for(;i!=childs.end();++i)
        if((*i)->getPriority()<priority)
            break;

    childs.insert(i,window);
}
