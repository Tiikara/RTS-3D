#ifndef CNOISE_H
#define CNOISE_H

#include <QGLShaderProgram>
#include "math.h"
#include <QtOpenGL>

struct noise_settings
{
    float start_kf;
    float grow_pow;

    float x0;
    float y0;
    float x1;
    float y1;

    float k,b;
    float p0[3];
    float p1[3];

    noise_settings():
        start_kf(1.001f),
        grow_pow(1.0f),
        x0(-0.5),
        y0(-0.5),
        x1(0.5),
        y1(0.5)
    {
        compute();
    }

    float corr_func(float x)
    {
        x*=1.41421;
        if (x<x0) return x*x*p0[0]+x*p0[1]+p0[2];
        if (x<x1) return x*k+b;
        return x*x*p1[0]+x*p1[1]+p1[2];
    }

    void compute()
    {
        k=(y1-y0)/(x1-x0);
        b=y0-k*x0;
        float zn=1.0+2.0*x0+x0*x0;
        p0[0]=(k+x0*k-y0-1.0)/zn;
        p0[1]=(-x0*x0*k+2.0*y0*x0+2.0*x0+k)/zn;
        p0[2]=(-x0*x0*k-x0*x0-x0*k+y0+2.0*y0*x0)/zn;

        zn=1.0-2.0*x1+x1*x1;
        p1[0]=(-1.0*k+x1*k-y1+1.0)/zn;
        p1[1]=(-x1*x1*k+2.0*y1*x1-2.0*x1+k)/zn;
        p1[2]=(x1*x1*k+x1*x1-x1*k+y1-2.0*y1*x1)/zn;
    }
};

class CNoise2D
{
    noise_settings nset;

    unsigned char Px[256]; //Таблица перестановок для x
    unsigned char Py[256]; //Таблица перестановок для y
    float G[256][2]; //Таблица единичных градиентов
    unsigned int seed; //Базовое число для генерации случайных чисел
    bool tex_ready; //Флаг того, что текстура готова к использованию
    GLuint tex; //Текстура с параметрами таблиц (r,g,b,a=Px,Py,G[0],G[1]), размер 256

    void generate_tables();
    float* grad(int x, int y);
public:
    CNoise2D();
    void gen(unsigned int seed);
    unsigned int get_seed();
    float operator()(float x, float y,float* n);
    float operator()(float x, float y);
    float operator()(float x, float y, float SF, float SA, int oct_n, float* n);

    float getHeight(float x, float y, int an, float A, float FR);
    void computeNormals(float x, float y, int an, float A, float FR, float *norm_out);

    void setCorrFunc(float start_kf, float grow_pow, float x0, float y0, float x1, float y1);

    void sendUValuesToShader(QGLShaderProgram &shader_prog);
    void enable_texture(); //Включает текстуру со случайными таблицами
    void disable_texture(); //Выключает текстуру со случайными таблицами
    void free_texture(); //Очищает память gpu (требует активный контекст opengl)
};

#endif // CNOISE_H
