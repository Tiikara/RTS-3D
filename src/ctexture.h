#ifndef CTEXTURE_H
#define CTEXTURE_H

#include <QtOpenGL>

class CTexture
{
public:
    CTexture();

    void loadFromFile(const char*fileName);

    void draw(float posX, float posY);

    float getWidth() { return width; }
    float getHeight() { return height; }
private:
    GLuint id;
    float width;
    float height;
};

#endif // CTEXTURE_H
