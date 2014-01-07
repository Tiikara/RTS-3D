#include "cbutton.h"

#include <QApplication>
#include <QWindow>
#include <QImage>
#include "GL/glu.h"

#include <QDebug>

CButton::CButton():
    selected(false),
    pressed(false),
    active(false)
{
}

void CButton::loadTextures(const char *normalTex, const char *selectedTex, const char *pressedTex)
{
    QGLContext *context = const_cast<QGLContext*>(QGLContext::currentContext());

    QImage img;

    img.load(normalTex);
    normalId=context->bindTexture(img);

    img.load(selectedTex);
    selectedId=context->bindTexture(img);

    img.load(pressedTex);
    pressedId=context->bindTexture(img);

    setWindowSize( img.width(), img.height() );
}

void CButton::draw()
{
    float *pos = getPosition();

    float width, height;

    getWindowSize(&width,&height);

    glEnable(GL_TEXTURE_2D);

    if(pressed)
    {
        glBindTexture(GL_TEXTURE_2D, pressedId);
    }
    else
    {
    if(selected)
    {
        glBindTexture(GL_TEXTURE_2D, selectedId);
    }
    else
    {
        glBindTexture(GL_TEXTURE_2D, normalId);
    }
    }

    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 0.0f); glVertex2f(pos[0],pos[1]);
    glTexCoord2f(1.0f, 0.0f); glVertex2f(pos[0]+width,pos[1]);
    glTexCoord2f(1.0f, 1.0f); glVertex2f(pos[0]+width,pos[1]+height);
    glTexCoord2f(0.0f, 1.0f); glVertex2f(pos[0],pos[1]+height);
    glEnd();

    glBindTexture(GL_TEXTURE_2D, 0);
    glDisable(GL_TEXTURE_2D);
}

void CButton::update()
{
    if(active)
    {
        selected = true;
    }
    else
    {
        selected = false;
        pressed = false;
    }

    active=false;

}

bool CButton::pressEvent(float, float, bool, bool)
{
    pressed = true;

    return true;
}

bool CButton::releaseEvent(float, float, bool, bool)
{
    emit clicked();
    pressed = false;

    return true;
}

bool CButton::moveEvent(float, float, bool, bool)
{
    return true;
}

bool CButton::positionEvent(float, float)
{
   active=true;

   return true;
}
