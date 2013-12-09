#ifndef CINTERFACE_H
#define CINTERFACE_H

#include "cwindow.h"
#include <QGLShaderProgram>
#include "GL/glu.h"

class CInterface : public CWindow
{
public:
    CInterface();

    void loadTextures(const char *tex);

    virtual void draw();
    virtual void update();
private:
    GLuint texId;
};

#endif // CINTERFACE_H
