#ifndef CMODEL_H
#define CMODEL_H

#include <qopengl.h>
#include "gl_ext/libExt.h"

class CModel
{
public:
    CModel();

    void loadFromFile(const char *fileName, const char *fileNameTexture);

    void draw();
private:
    GLuint vId;
    GLuint iId;
    GLuint tId;
    GLuint tcId;
    GLuint nId;
    GLsizei iCount;
};

#endif // CMODEL_H
