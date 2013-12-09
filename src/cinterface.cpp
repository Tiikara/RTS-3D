#include "cinterface.h"

#include "cbutton.h"

#include <QApplication>
#include <QWindow>

CInterface::CInterface()
{
}

void CInterface::loadTextures(const char *tex)
{
    QGLContext *context = const_cast<QGLContext*>(QGLContext::currentContext());

    QImage img;

    img.load(tex);
    texId=context->bindTexture(img);

    setWindowSize( img.width(), img.height() );


    CButton *button = new CButton;
    button->loadTextures("tex/butNormal.png","tex/butSelected.png","tex/butPressed.png");

    addChild(button, 290, 60, 2);
}

void CInterface::draw()
{
    float *pos = getPosition();

    float width, height;

    getWindowSize(&width,&height);

    glEnable(GL_TEXTURE_2D);

    glBindTexture(GL_TEXTURE_2D, texId);

    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 0.0f); glVertex2f(pos[0],pos[1]);
    glTexCoord2f(1.0f, 0.0f); glVertex2f(pos[0]+width,pos[1]);
    glTexCoord2f(1.0f, 1.0f); glVertex2f(pos[0]+width,pos[1]+height);
    glTexCoord2f(0.0f, 1.0f); glVertex2f(pos[0],pos[1]+height);
    glEnd();

    glBindTexture(GL_TEXTURE_2D, 0);
    glDisable(GL_TEXTURE_2D);

    CWindow::draw();
}

void CInterface::update()
{

    CWindow::update();
}
