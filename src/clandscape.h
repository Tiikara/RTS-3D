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
#include "cforest.h"

typedef float vec3f[3];
typedef float quad[4][3];

class CLandscape
{
public:
    CLandscape();

    void initializeGL();

    void loadSettings(const char *fileName);
    void generateLandscape();
    float getHeight(float x, float y);
    int getSurfaceType(float x, float y);
    void getNormal(float x, float y, vec3f norm_out);

    void draw(float *positionCamera);
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
    CForest forest;
    CWater water;

    // settings
    float fWidth;
    float fHeight;
    float fSide;
    int layer_c;
    float flayer_c;
    int border_c;
    float *borders;
    GLuint bordersId;
    // ---

    UINT uiSeed;

    GLuint tex_id;
    QGLShaderProgram shader_prog;
    CNoise2D noise;

    void genTexture();

};

#endif // LANDSCAPE_H
