#include "cwater.h"

#include <time.h>

CWater::CWater():
    fSeaLevel(0.0f)
{
}

void CWater::initializeGL()
{
    noise.gen(time(NULL));

    water_shader.addShaderFromSourceFile(QGLShader::Vertex,":/shaders/water.vert");
    water_shader.addShaderFromSourceFile(QGLShader::Fragment,":/shaders/water.frag");
    water_shader.link();
    water_shader.bind();
    water_shader.setUniformValue("sky_map",0);
    water_shader.setUniformValue("noise_table",1);
    water_shader.release();

    QImage img("tex/sky.jpg");
    QGLContext *context = const_cast<QGLContext*>(QGLContext::currentContext());
    sky_tex=context->bindTexture(img);
}

void CWater::setSeaLevel(float level)
{
    fSeaLevel = level;
}

float CWater::getSeaLevel()
{
    return fSeaLevel;
}

void CWater::draw(float *cam_pos)
{
    glActiveTextureARB(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D,sky_tex);
    glEnable(GL_TEXTURE_2D);
    glActiveTextureARB(GL_TEXTURE1);
    noise.enable_texture();

    water_shader.bind();
    water_shader.setUniformValue("view_pos",cam_pos[0],cam_pos[1],cam_pos[2]);
    water_shader.setUniformValue("l0_dv",sa.pos0[0],sa.pos0[1]);
    water_shader.setUniformValue("l1_dv",sa.pos1[0],sa.pos1[1]);
    water_shader.setUniformValue("l2_dv",sa.pos2[0],sa.pos2[1]);

    glBegin(GL_QUADS);
        glVertex3f(-50.0,-50.0,fSeaLevel);
        glVertex3f(50.0,-50.0,fSeaLevel);
        glVertex3f(50.0,50.0,fSeaLevel);
        glVertex3f(-50.0,50.0,fSeaLevel);
    glEnd();

    water_shader.release();

    noise.disable_texture();
    glActiveTextureARB(GL_TEXTURE0);
    glDisable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D,0);
}

void CWater::update()
{
    sa.computeFrame(0.03);
}


CWater::cSeaAnim::cSeaAnim()
{
    buildRndVec(l0_dv,1.0);
    buildRndVec(l1_dv,2.0);
    buildRndVec(l2_dv,4.0);
    pos0[0]=0.0; pos0[1]=0.0;
    pos1[0]=0.0; pos1[1]=0.0;
    pos2[0]=0.0; pos2[1]=0.0;
}


void CWater::cSeaAnim::buildRndVec(float *v, float sz)
{
    v[0]=rand()/(float)RAND_MAX-0.5f;
    v[1]=rand()/(float)RAND_MAX-0.5f;
    float l=sz/sqrtf(v[0]*v[0]+v[1]*v[1]);
    v[0]*=l;
    v[1]*=l;
}

void CWater::cSeaAnim::computeFrame(float dt)
{
    float max=100000;
    pos0[0]+=dt*l0_dv[0];    pos0[1]+=dt*l0_dv[1];
    pos1[0]+=dt*l1_dv[0];    pos1[1]+=dt*l1_dv[1];
    pos2[0]+=dt*l2_dv[0];    pos2[1]+=dt*l2_dv[1];
    if (fabs(pos0[0])>max) l0_dv[0]=-l0_dv[0];
    if (fabs(pos0[1])>max) l0_dv[1]=-l0_dv[1];
    if (fabs(pos1[0])>max) l1_dv[0]=-l1_dv[0];
    if (fabs(pos1[1])>max) l1_dv[1]=-l1_dv[1];
    if (fabs(pos2[0])>max) l2_dv[0]=-l2_dv[0];
    if (fabs(pos2[1])>max) l2_dv[1]=-l2_dv[1];
}
