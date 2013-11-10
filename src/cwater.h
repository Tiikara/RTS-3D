#ifndef CWATER_H
#define CWATER_H

#include <QGLShaderProgram>
#include <QGLWidget>
#include "GL/glu.h"
#include "gl_ext/libExt.h"
#include "cnoise.h"

class CWater
{
public:
    CWater();

    void initializeGL();

    void setSeaLevel(float level);
    float getSeaLevel();

    void draw( float *cam_pos );
    void update();
private:
    struct cSeaAnim
    {
        float l0_dv[2];
        float l1_dv[2];
        float l2_dv[2];
        float pos0[2];
        float pos1[2];
        float pos2[2];

        cSeaAnim();
        void buildRndVec(float *v,float sz=1.0f);
        void computeFrame(float dt);
    };

    QGLShaderProgram water_shader;
    GLuint sky_tex;
    cSeaAnim sa;
    CNoise2D noise;

    float fSeaLevel;
};

#endif // CWATER_H
