#include "clandscape.h"

#include <QSettings>

#include <time.h>

CLandscape::CLandscape():
    fSeaLevel(0.0)
{}

void CLandscape::initializeGL()
{
    mesh.generate(fWidth,fHeight,fSide);
    noise.gen(uiSeed);

    shader_prog.addShaderFromSourceFile(QGLShader::Vertex,"surf_vert.vert");
    shader_prog.addShaderFromSourceFile(QGLShader::Fragment,"surf_frag.frag");

    shader_prog.link(); // линкуем шейдеры
    shader_prog.bind();
    shader_prog.setUniformValue("freq",5.0f);
    shader_prog.setUniformValue("size",1.5f);
    shader_prog.setUniformValue("tex",0);
    shader_prog.setUniformValue("noise_table",1);
    shader_prog.release();

    genTexture();
}

void CLandscape::genTexture()
{
    const int n=3;
    unsigned char* tex;
    char fname[256];
    int w;
    int h;
    int d=n;

    int id=0;
    for (int i=0; i<3; i++)
    {
        sprintf(fname,"tex/layer_%d.jpg",i);
        QImage img;
        img.load(QString(fname));
        if (i==0) //Размер ещё не был известен, память не выделена
        {
            w=img.width();
            h=img.height();
            tex=new unsigned char[3*w*h*d];
        }
        //Записываем значения цветов в tex
        QRgb color;
        for (int i=0; i<h; i++)
        {
            for (int j=0; j<w; j++)
            {
                color=img.pixel(j,i);
                tex[id++]=qRed(color);
                tex[id++]=qGreen(color);
                tex[id++]=qBlue(color);
            }
        }
    }
    //Генерируем openGL текстуру
    glGenTextures(1,&tex_id);
    glBindTexture(GL_TEXTURE_3D_EXT,tex_id);
    //Обязательные параметры фильтрации
    glTexParameteri(GL_TEXTURE_3D_EXT,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_3D_EXT,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    //Чтобы текстура тайлилась по x и y:
    glTexParameteri(GL_TEXTURE_3D_EXT,GL_TEXTURE_WRAP_S,GL_REPEAT);
    glTexParameteri(GL_TEXTURE_3D_EXT,GL_TEXTURE_WRAP_T,GL_REPEAT);
    //Загружаем изображение в видеопамять
    glTexImage3DEXT(GL_TEXTURE_3D_EXT,0,GL_RGB,w,h,d,0,GL_RGB,GL_UNSIGNED_BYTE,tex);
    //Больше tex не нужен
    delete[] tex;
}

void CLandscape::loadSettings(const char *fileName)
{
    QSettings settings(fileName, QSettings::IniFormat);

    settings.beginGroup("Global");
    fSeaLevel = settings.value("SeaLevel").toFloat();
    settings.endGroup();

    settings.beginGroup("Size");
    fWidth = settings.value("Width").toFloat();
    fHeight = settings.value("Height").toFloat();
    fSide = settings.value("Side").toFloat();
    settings.endGroup();

    settings.beginGroup("Generator");
    uiSeed = settings.value("Seed").toUInt();
    noise.setCorrFunc( settings.value("start_kf").toFloat(),
                       settings.value("grow_pow").toFloat(),
                       settings.value("x0").toFloat(),
                       settings.value("y0").toFloat(),
                       settings.value("x1").toFloat(),
                       settings.value("y1").toFloat());
    settings.endGroup();
}

void CLandscape::generateLandscape()
{
    noise.gen(time(NULL));
}

float CLandscape::getHeight(float x, float y)
{
    return noise.getHeight(x,y,10,1.5f,0.5);
}

void CLandscape::getNormal(float x, float y, vec3f norm_out)
{
    noise.computeNormals(x,y,10,1.5f,0.5,norm_out);
}

void CLandscape::draw(float *cam_pos)
{
    glActiveTextureARB(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_3D_EXT,tex_id);
    glEnable(GL_TEXTURE_3D_EXT);
    glActiveTextureARB(GL_TEXTURE1);
    noise.enable_texture();

    shader_prog.bind();
    shader_prog.setUniformValue("view_pos",cam_pos[0],cam_pos[1],cam_pos[2]);
    noise.sendUValuesToShader(shader_prog);

    mesh.draw();
    shader_prog.release();

    noise.disable_texture();
    glActiveTextureARB(GL_TEXTURE0);
    glDisable(GL_TEXTURE_3D_EXT);
    glBindTexture(GL_TEXTURE_3D_EXT,0);

    glColor4f(0.0,0.4,0.8,0.8);
    glBegin(GL_QUADS);

    glVertex3f(-5.0,-5.0,fSeaLevel);
    glVertex3f(5.0,-5.0,fSeaLevel);
    glVertex3f(5.0,5.0,fSeaLevel);
    glVertex3f(-5.0,5.0,fSeaLevel);

    glEnd();
}


void CLandscape::cSurfaceMesh::generate(float w, float h, float side)
{
    int ws=(int)w/side;
    int hs=(int)h/side;
    float s_w=w/(float)ws;
    float s_h=h/(float)hs;
    int n=ws*hs;
    quad* q=new quad[n];
    int id=0;
    for (int i=0; i<hs; i++)
    {
        for (int j=0; j<ws; j++)
        {
            q[id][0][0]=j*s_w-w*0.5f;
            q[id][0][1]=i*s_h-h*0.5f;
            q[id][0][2]=0.0f;

            q[id][1][0]=(j+1)*s_w-w*0.5f;
            q[id][1][1]=i*s_h-h*0.5f;
            q[id][1][2]=0.0f;

            q[id][2][0]=(j+1)*s_w-w*0.5f;
            q[id][2][1]=(i+1)*s_h-h*0.5f;
            q[id][2][2]=0.0f;

            q[id][3][0]=j*s_w-w*0.5f;
            q[id][3][1]=(i+1)*s_h-h*0.5f;
            q[id][3][2]=0.0f;
            id++;
        }
    }

    vCount = n*4;

    glGenBuffersARB( 1, &vId );
    glBindBufferARB(GL_ARRAY_BUFFER, vId );
    glBufferDataARB(GL_ARRAY_BUFFER, sizeof(float)*3*4*n, q, GL_STATIC_DRAW);
    glBindBufferARB(GL_ARRAY_BUFFER,0);

    delete[] q;
}

void CLandscape::cSurfaceMesh::draw()
{
    glEnableClientState( GL_VERTEX_ARRAY );
    glBindBufferARB(GL_ARRAY_BUFFER, vId);
    glVertexPointer( 3, GL_FLOAT, 0, NULL );
    glDrawArrays(GL_QUADS, 0, vCount);
    glDisableClientState(GL_VERTEX_ARRAY);
}
