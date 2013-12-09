#include "clandscape.h"

#include <QSettings>

#include <time.h>
#include <algorithm>

#include "ccamera.h"

#define PI 3.14159265f

CLandscape::CLandscape()
{}

void CLandscape::initializeGL()
{
    mesh.generate(fWidth,fHeight,fSide);
    noise.gen(uiSeed);

    shader_prog.addShaderFromSourceFile(QGLShader::Vertex,":/shaders/surface.vert");
    shader_prog.addShaderFromSourceFile(QGLShader::Fragment,":/shaders/surface.frag");

    shader_prog.link();
    shader_prog.bind();
    shader_prog.setUniformValue("amplit",fAmplit);
    shader_prog.setUniformValue("freq",fFreq);
    shader_prog.setUniformValue("tex",0);
    shader_prog.setUniformValue("noise_table",1);
    shader_prog.setUniformValue("tex1Borders",2);
    shader_prog.setUniformValue("flayerC",flayer_c);
    shader_prog.setUniformValue("layerC",layer_c);
    shader_prog.release();

    genTexture();

    water.initializeGL();
}

void CLandscape::setCamera(CCamera *camera)
{
    this->camera = camera;
}

void CLandscape::genTexture()
{
    const int n=layer_c;
    unsigned char* tex;
    char fname[256];
    int w;
    int h;
    int d=n;

    int id=0;
    for (int i=0; i<layer_c; i++)
    {
        sprintf(fname,"tex/layer_%d.jpg",i);
        QImage img;
        img.load(QString(fname));
        if (i==0)
        {
            w=img.width();
            h=img.height();
            tex=new unsigned char[3*w*h*d];
        }

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

    glGenTextures(1,&tex_id);
    glBindTexture(GL_TEXTURE_3D_EXT,tex_id);
    glTexParameteri(GL_TEXTURE_3D_EXT,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_3D_EXT,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_3D_EXT,GL_TEXTURE_WRAP_S,GL_REPEAT);
    glTexParameteri(GL_TEXTURE_3D_EXT,GL_TEXTURE_WRAP_T,GL_REPEAT);
    glTexImage3DEXT(GL_TEXTURE_3D_EXT,0,GL_RGB,w,h,d,0,GL_RGB,GL_UNSIGNED_BYTE,tex);
    glBindTexture(GL_TEXTURE_3D_EXT,0);
    delete[] tex;

    glGenTextures(1,&bordersId);
    glBindTexture(GL_TEXTURE_1D,bordersId);
    glTexParameteri(GL_TEXTURE_1D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
    glTexParameteri(GL_TEXTURE_1D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
    glTexParameteri(GL_TEXTURE_1D,GL_TEXTURE_WRAP_S,GL_REPEAT);
    glTexImage1D(GL_TEXTURE_1D,0,GL_R32F,border_c,0,GL_RED,GL_FLOAT,borders);
    glBindTexture(GL_TEXTURE_1D,0);
}

void CLandscape::loadSettings(const char *fileName)
{
    QSettings settings(fileName, QSettings::IniFormat);

    settings.beginGroup("Global");
    water.setSeaLevel(settings.value("SeaLevel").toFloat());
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
    fFreq = settings.value("Frequency").toFloat();
    fAmplit = settings.value("Amplitude").toFloat();
    settings.endGroup();

    settings.beginGroup("Layer_c");
    flayer_c = settings.value("flayer_c").toFloat();
    layer_c = settings.value("layer_c").toInt();
    settings.endGroup();

    border_c = layer_c-1;

    borders = new float[border_c];

    settings.beginGroup("TextureBorders");
    for(int i=0;i<border_c;i++)
        borders[i] = settings.value("Border." + QString::number(i)).toFloat();
    settings.endGroup();
}

void CLandscape::generateLandscape()
{
    noise.gen(time(NULL));
}

float CLandscape::getHeight(float x, float y)
{
    return std::max(water.getSeaLevel(),
                    noise.getHeight(x,y,10,fAmplit,fFreq));
}

int CLandscape::getSurfaceType(float x, float y)
{
    float h = getHeight(x,y);
    return getSurfaceType(h);
}

int CLandscape::getSurfaceType(float h)
{
    if(h <= water.getSeaLevel())
        return 0;

    for(int i=0;i<border_c;i++)
    {
        if(h < borders[i])
            return (i+1);
    }

    return layer_c;
}

void CLandscape::getNormal(float x, float y, vec3f norm_out)
{
    noise.computeNormals(x,y,10,fAmplit,fFreq,norm_out);
}

void CLandscape::getIntersectPosition(float mx, float my, float *pos)
{
    float *cam_pos = camera->getPosition();

    my = QApplication::activeWindow()->height() - my;
    float tray[6];

    float rotate = camera->getRotate();
    float angle = -camera->getAngle();

    qDebug() << "angle: " << angle;

    my = (-1.0f + (2.0f/QApplication::activeWindow()->height())*(my));
    mx = -(-1.0f + (2.0f/QApplication::activeWindow()->width())*(mx));

    qDebug() << "mx: " << mx << " " << "my: " << my;

    float sinv = sin(rotate * PI/180.0f);
    float cosv = cos(rotate * PI/180.0f);

//    tray[0] = cosv*mx;
//    tray[1] = sinv*mx;
//    tray[2] = my;

//    tray[3+0] = 0.5f * cosv - mx * sinv;
//    tray[3+1] = mx * cosv + 0.5f * sinv;
//    tray[3+2] = my;

    tray[0] = 0;
    tray[1] = mx;
    tray[2] = my;

    tray[3+0] = 0.5;
    tray[3+1] = mx;
    tray[3+2] = my;

//    tray[0] = 0;
//    tray[1] = mx;
//    tray[2] = my;

//    tray[3+0] = 0+0.5f;
//    tray[3+1] = mx;
//    tray[3+2] = my;

    sinv = sin(angle * PI/180.0f);
    cosv = cos(angle * PI/180.0f);

    float ray[6];

    ray[0] = tray[0]*cosv-tray[2]*sinv + cam_pos[0];
    ray[1] = tray[1] + cam_pos[1];
    ray[2] = tray[0]*sinv+tray[2]*cosv + cam_pos[2];

    ray[3+0] = tray[3+0]*cosv-tray[3+2]*sinv + cam_pos[0];
    ray[3+1] = tray[3+1] + cam_pos[1];
    ray[3+2] = tray[3+0]*sinv+tray[3+2]*cosv + cam_pos[2];

    //ray[0] = tray[0]*cosv+;

    qDebug() << "vector1: " << ray[0] << " " << ray[1] << " " << ray[2];
    qDebug() << "vector1: " << ray[3+0] << " " << ray[3+1] << " " << ray[3+2];

    QVector3D rayStep(ray[3+0]-ray[0],ray[3+1]-ray[1],ray[3+2]-ray[2]);
    QVector3D rayStartPosition(ray[0],ray[1],ray[2]);
    QVector3D rayPosition = rayStartPosition;

    QVector3D lastRayPosition = rayStartPosition;
    rayPosition += rayStep;
    float height = getHeight(rayPosition.x(),rayPosition.y());

    while (rayPosition.z() >= height)
    {
        lastRayPosition = rayPosition;
        rayPosition += rayStep;
        height = getHeight(rayPosition.x(),rayPosition.y());
    }

    QVector3D startPosition = lastRayPosition;
    QVector3D endPosition = rayPosition;

    for (int i = 0; i < 32; i++)
    {
        QVector3D middlePoint = (startPosition + endPosition) * 0.5f;

        if (middlePoint.z() < height)
            endPosition = middlePoint;
        else
            startPosition = middlePoint;
    }

    QVector3D collisionPoint = (startPosition + endPosition) * 0.5f;

    pos[0] = collisionPoint.x();
    pos[1] = collisionPoint.y();
    pos[2] = height;

    qDebug() << "res: " << pos[0] << " " << pos[1] << " " << pos[2];

}

void CLandscape::draw()
{
    float *cam_pos = camera->getPosition();

    glActiveTextureARB(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_3D_EXT,tex_id);
    glEnable(GL_TEXTURE_3D_EXT);
    glActiveTextureARB(GL_TEXTURE1);
    noise.enable_texture();
    glEnable(GL_TEXTURE_1D);
    glActiveTextureARB(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_1D,bordersId);

    shader_prog.bind();
    shader_prog.setUniformValue("view_pos",cam_pos[0],cam_pos[1],cam_pos[2]);
    noise.sendUValuesToShader(shader_prog);

    mesh.draw();
    shader_prog.release();

    noise.disable_texture();
    glActiveTextureARB(GL_TEXTURE0);
    glDisable(GL_TEXTURE_3D_EXT);
    glBindTexture(GL_TEXTURE_3D_EXT,0);
    glDisable(GL_TEXTURE_1D);
    glBindTexture(GL_TEXTURE_1D,0);

    water.draw(cam_pos);
}

void CLandscape::update()
{
    water.update();
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
