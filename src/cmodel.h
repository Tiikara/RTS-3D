#ifndef CMODEL_H
#define CMODEL_H

#include <QGLShaderProgram>
#include "gl_ext/libExt.h"

class CModel
{
public:
    CModel();

    void loadFromFile(const char *fileName, const char *fileNameTexture);

    void initializeGL();

    void draw(float *cam_pos);
private:
    QGLShaderProgram model_shader;

    GLuint vId;
    GLuint iId;
    GLuint tId;
    GLuint tcId;
    GLuint nId;
    GLsizei iCount;
};

#endif // CMODEL_H
