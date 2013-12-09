#ifndef LANDSCAPE_H
#define LANDSCAPE_H

#include <QGLShaderProgram>
#include <QGLWidget>
#include "GL/glu.h"
#include "gl_ext/libExt.h"
#include "math.h"
#include "cnoise.h"
#include <QVector3D>
#include "cwater.h"

typedef float vec3f[3];
typedef float quad[4][3];

class CCamera;

class CLandscape
{
public:
    CLandscape();

    void initializeGL();

    void setCamera(CCamera *camera);

    void loadSettings(const char *fileName);
    void generateLandscape();
    float getHeight(float x, float y);
    int getSurfaceType(float x, float y);
    int getSurfaceType(float height);
    void getNormal(float x, float y, vec3f norm_out);

    void getIntersectPosition(float mx, float my, float *pos);

    void draw();
    void update();
private:
    struct cSurfaceMesh
    {
        GLuint vId;
        GLsizei vCount;

        void generate(float w, float h, float side);
        void draw();
    };

    cSurfaceMesh mesh;
    CWater water;
    CCamera *camera;

    // settings
    float fWidth;
    float fHeight;
    float fSide;
    int layer_c;
    float flayer_c;
    int border_c;
    float *borders;
    GLuint bordersId;

    float fFreq;
    float fAmplit;
    // ---

    UINT uiSeed;

    GLuint tex_id;
    QGLShaderProgram shader_prog;
    CNoise2D noise;

    void genTexture();

};

#endif // LANDSCAPE_H
