#include "ctexture.h"

#include "MainWindow.h"

CTexture::CTexture()
{
}

void CTexture::loadFromFile(const char *fileName)
{
    QGLContext *context = const_cast<QGLContext*>(QGLContext::currentContext());
    QImage img;
    img.load(fileName);
    id=context->bindTexture(img);

    width = img.width();
    height = img.height();
}

void CTexture::draw(float posX, float posY)
{
    glEnable(GL_TEXTURE_2D);

    glBindTexture(GL_TEXTURE_2D, id);

    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 0.0f); glVertex2f(posX,posY);
    glTexCoord2f(1.0f, 0.0f); glVertex2f(posX+width,posY);
    glTexCoord2f(1.0f, 1.0f); glVertex2f(posX+width,posY+height);
    glTexCoord2f(0.0f, 1.0f); glVertex2f(posX,posY+height);
    glEnd();

    glBindTexture(GL_TEXTURE_2D, 0);
    glDisable(GL_TEXTURE_2D);
}

void CTexture::begin2DMode()
{
    glDisable(GL_DEPTH_TEST);

    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    gluOrtho2D( 0,MainWindow::getWidth(),0,MainWindow::getHeight());
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void CTexture::end2DMode()
{
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);

    glEnable(GL_DEPTH_TEST);
}
