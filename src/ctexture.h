#ifndef CTEXTURE_H
#define CTEXTURE_H

#include <QtOpenGL>
#include "GL/glu.h"

class CTexture
{
public:
    CTexture();

    void loadFromFile(const char*fileName);

    void draw(float posX, float posY);

    static void begin2DMode();
    static void end2DMode();

    float getWidth() const { return width; }
    float getHeight() const { return height; }
private:
    GLuint id;
    float width;
    float height;
};

#endif // CTEXTURE_H
