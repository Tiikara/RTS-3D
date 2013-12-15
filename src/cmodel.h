#ifndef CMODEL_H
#define CMODEL_H

#include <QGLShaderProgram>
#include "gl_ext/libExt.h"

typedef unsigned int ui;
typedef float vec3f[3];

class CModel
{
public:
    CModel();

    void loadFromFile(const char *fileName, const char *fileNameTexture);
    void loadAnimationDesc(const char *fileName);

    void initializeGL();

    void setPosition(float *position);

    void setPosition(float x, float y, float z);
    void setScale(float x, float y, float z);
    void setRotateParam(float angle, float x, float y, float z);

    void draw(float frame = 0);
private:
    QMatrix4x4 modelViewMatrix;
    vec3f pos;
    float rot[4];
    vec3f scale;

    QGLShaderProgram model_shader;

    int vertexLocation;
    int normalLocation;

    ui countFrames;

    GLuint *vId;
    GLuint *nId;
    GLuint iId;
    GLuint tId;
    GLuint tcId;
    GLsizei iCount;
};

#endif // CMODEL_H
