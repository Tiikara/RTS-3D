#ifndef CMODEL_H
#define CMODEL_H

#include <QGLShaderProgram>
#include "gl_ext/libExt.h"

typedef unsigned int ui;

class CModel
{
public:
    CModel();

    void loadFromFile(const char *fileName, const char *fileNameTexture);

    void initializeGL();

    void draw(float *cam_pos);
private:
    QGLShaderProgram model_shader;

    int vertexLocation;
    int normalLocation;

    ui countFrames;
    float fCurrentFrame;

    GLuint *vId;
    GLuint *nId;
    GLuint iId;
    GLuint tId;
    GLuint tcId;
    GLsizei iCount;
};

#endif // CMODEL_H
