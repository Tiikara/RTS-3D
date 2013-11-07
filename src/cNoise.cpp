#include "cNoise.h"

#include <QVector3D>

typedef float vec3f[3];

cNoise2D::cNoise2D():
    tex_ready(false),
    tex(0)
{}
void cNoise2D::gen(unsigned int seed)
{
    tex_ready=false;
    this->seed=seed;
    generate_tables();
}
void cNoise2D::generate_tables()
{
    srand(seed);
    for (int i=0; i<256; i++) Px[i]=Py[i]=i;
    int  v,id;
    for (int i=0; i<256; i++)
    {
        id=rand()%256;
        v=Px[id];
        Px[id]=Px[i];
        Px[i]=v;

        id=rand()%256;
        v=Py[id];
        Py[id]=Py[i];
        Py[i]=v;
    }
    float r;
    for (int i=0; i<256; i++)
    {
        do
        {
            G[i][0]=2.0f*(float)rand()/(float)RAND_MAX-1.0f;
            G[i][1]=2.0f*(float)rand()/(float)RAND_MAX-1.0f;
            r=G[i][0]*G[i][0]+G[i][1]*G[i][1];
        }
        while (r>1.0f);
        r=1.0f/sqrtf(r);
        G[i][0]*=r;
        G[i][1]*=r;
    }
}
float* cNoise2D::grad(int x, int y)
{
    return G[(unsigned char)(Px[(unsigned char)x]+Py[(unsigned char)y])];
}
unsigned int cNoise2D::get_seed()
{
    return seed;
}
float cNoise2D::operator()(float x, float y)
{
    int x0=(int)floor(x);
    int y0=(int)floor(y);
    int x1=x0+1;
    int y1=y0+1;
    float *g;

    float s,t,u,v;
    g=grad(x0,y0); s=g[0]*(x-(float)x0)+g[1]*(y-(float)y0);
    g=grad(x1,y0); t=g[0]*(x-(float)x1)+g[1]*(y-(float)y0);
    g=grad(x0,y1); u=g[0]*(x-(float)x0)+g[1]*(y-(float)y1);
    g=grad(x1,y1); v=g[0]*(x-(float)x1)+g[1]*(y-(float)y1);

    float d2=(x-(float)x0);
    d2*=d2;
    float S=3.0*d2-2.0*d2*(x-(float)x0);
    float a=s+S*(t-s);
    float b=u+S*(v-u);

    d2=y-(float)y0;
    d2*=d2;
    S=3.0*d2-2.0*d2*(y-(float)y0);

    return a+S*(b-a);
}
float cNoise2D::operator()(float x, float y, float* n)
{
    int x0=(int)floor(x);
    int y0=(int)floor(y);
    int x1=x0+1;
    int y1=y0+1;
    float *g[4];

    float s,t,u,v;
    g[0]=grad(x0,y0); s=g[0][0]*(x-(float)x0)+g[0][1]*(y-(float)y0);
    g[1]=grad(x1,y0); t=g[1][0]*(x-(float)x1)+g[1][1]*(y-(float)y0);
    g[2]=grad(x0,y1); u=g[2][0]*(x-(float)x0)+g[2][1]*(y-(float)y1);
    g[3]=grad(x1,y1); v=g[3][0]*(x-(float)x1)+g[3][1]*(y-(float)y1);

    float dx2=(x-(float)x0);
    dx2*=dx2;
    float Sx=3.0*dx2-2.0*dx2*(x-(float)x0);
    float a=s+Sx*(t-s);
    float b=u+Sx*(v-u);

    float dy2=y-(float)y0;
    dy2*=dy2;
    float Sy=3.0*dy2-2.0*dy2*(y-(float)y0);

    float v0,v1,v2,v3,v4; //Вспомогательные величины для расчёта нормали
    v0=6.0f*x-6.0f*(float)x0-6.0f*dx2;
    v1=v0*(t-s);
    v2=Sx*(g[1][0]-g[0][0]);
    v3=6.0f*y-6.0f*(float)y0-6.0f*dy2;
    v4=Sx*(g[1][1]-g[0][1]);

    n[0]=-(g[0][0]+v1+v2+Sy*(g[2][0]+v0*(v-u)+Sx*(g[3][0]-g[2][0])-g[0][0]-v1-v2));
    n[1]=-(g[0][1]+v4+v3*(u+Sx*(v-u)-s-Sx*(t-s))+Sy*(g[2][1]+Sx*(g[3][1]-g[2][1])-g[0][1]-v4));
    n[2]=1.0f;

    return a+Sy*(b-a);
}
float cNoise2D::operator()(float x, float y, float SF, float SA, int oct_n, float* n)
{
    float h=0.0f;
    n[0]=0.0f; n[1]=0.0f; n[2]=1.0f;
    float nb[3];
    for (int i=0; i<oct_n; i++)
    {
        h+=SA*(*this)(x*SF,y*SF,nb);
        n[0]+=nb[0]*SA*SF;
        n[1]+=nb[1]*SA*SF;
        SA*=0.5f;
        SF*=2.0f;
    }
    return h;
}


float cNoise2D::getHeight(float x, float y, int an, float A, float FR)
{
    float cur_a=A;
    float cur_fr=FR;
    float h=0.0;
    float cur_hscale=1.0;
    float step_max_h=A;
    for (int i=0; i<an; i++)
    {
        cur_a*=cur_hscale;
        h+=nset.corr_func( (*this)(x*cur_fr,y*cur_fr))*cur_a;
        cur_hscale=pow(1.0-(step_max_h-abs(h))/(nset.start_kf*step_max_h),nset.grow_pow);
        cur_fr*=2.0;
        cur_a*=0.5;
    }

    return h;
}

void cNoise2D::computeNormals(float x, float y, int an, float A, float FR, float *norm_out)
{
    float cur_a=A;
    float cur_fr=FR;
    float h=0.0;
    vec3f n;
    float cur_hscale=1.0;
    float step_max_h=A;
    QVector3D norm(0.0,0.0,1.0);
    for (int i=0; i<an; i++)
    {
        cur_a*=cur_hscale;
        h+=nset.corr_func( (*this)(x*cur_fr,y*cur_fr, n))*cur_a;
        cur_hscale=pow(1.0-(step_max_h-abs(h))/(nset.start_kf*step_max_h),nset.grow_pow);
        norm.setX(norm.x()+n[0]*cur_a*cur_fr);
        norm.setY(norm.y()+n[1]*cur_a*cur_fr);
        cur_fr*=2.0;
        cur_a*=0.5;
    }

    norm.normalize();
    norm_out[0] = norm.x();
    norm_out[1] = norm.y();
    norm_out[2] = norm.z();
}

void cNoise2D::setCorrFunc(float start_kf, float grow_pow, float x0, float y0, float x1, float y1)
{
    nset.start_kf = start_kf;
    nset.grow_pow = grow_pow;
    nset.x0 = x0;
    nset.y0 = y0;
    nset.x1 = x1;
    nset.y1 = y1;
    nset.compute();
}

void cNoise2D::sendUValuesToShader(QGLShaderProgram &shader_prog)
{
    shader_prog.setUniformValue("start_kf",nset.start_kf);
    shader_prog.setUniformValue("grow_pow",nset.grow_pow);
    shader_prog.setUniformValue("x_val",nset.x0,nset.x1);
    shader_prog.setUniformValue("curve0",nset.p0[0],nset.p0[1],nset.p0[2]);
    shader_prog.setUniformValue("curve1",nset.p1[0],nset.p1[1],nset.p1[2]);
    shader_prog.setUniformValue("kb_line",nset.k,nset.b);
}
void cNoise2D::enable_texture()
{
    if (!tex_ready) //Нужно сначала создать текстуру
    {
        if (tex) glDeleteTextures(1,&tex);
        glGenTextures(1,&tex);
        glBindTexture(GL_TEXTURE_1D,tex);
        glTexParameteri(GL_TEXTURE_1D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
        glTexParameteri(GL_TEXTURE_1D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
        glTexParameteri(GL_TEXTURE_1D,GL_TEXTURE_WRAP_S,GL_REPEAT);
        float tbl[256][4];
        for (int i=0; i<256; i++)
        {
            tbl[i][0]=Px[i]/255.0f;
            tbl[i][1]=Py[i]/255.0f;
            tbl[i][2]=G[i][0]*0.5f+0.5f;
            tbl[i][3]=G[i][1]*0.5f+0.5f;
        }
        glTexImage1D(GL_TEXTURE_1D,0,GL_RGBA,256,0,GL_RGBA,GL_FLOAT,(float*)tbl);
        tex_ready=true;
    }
    else glBindTexture(GL_TEXTURE_1D,tex);
    glEnable(GL_TEXTURE_1D);
}
void cNoise2D::disable_texture()
{
    glBindTexture(GL_TEXTURE_1D,0);
    glDisable(GL_TEXTURE_2D);
}
void cNoise2D::free_texture()
{
    if (tex) glDeleteTextures(1,&tex);
    tex_ready=false;
}
