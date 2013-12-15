#include "ctexture.h"

CTexture::CTexture()
{
}

void CTexture::loadFromFile(const char *fileName)
{
    QGLContext *context = const_cast<QGLContext*>(QGLContext::currentContext());
    QImage img;
    img.load(fileName);
    id=context->bindTexture(img);
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
