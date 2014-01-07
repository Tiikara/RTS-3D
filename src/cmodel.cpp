#include "cmodel.h"
#include <stdio.h>

#include <QGLContext>
#include <QImage>
#include <QFile>


CModel::CModel()
{
    for(int i=0;i<3;i++)
        pos[i] = 0.0f;

    for(int i=0;i<3;i++)
        scale[i] = 1.0f;

    for(int i=0;i<4;i++)
        rot[i] = 0.0f;
}

void CModel::loadFromFile(const char *fileName, const char *fileNameTexture)
{
    QImage img(fileNameTexture);
    QGLContext *context = const_cast<QGLContext*>(QGLContext::currentContext());
    tId=context->bindTexture(img);

    FILE *pFile = fopen (fileName , "rb");

    fseek(pFile, 0L, SEEK_END);
    const ui fileSize = ftell(pFile);
    fseek(pFile, 0L, SEEK_SET);

    void **buffer = new void*[fileSize];

    fread(buffer, 4,fileSize,pFile);

    fclose(pFile);

    ui countVert  = (ui)buffer[10];
    ui countIndex = (ui)buffer[11];
    countFrames = (ui)buffer[12];

    ui offset = 13;
    ui    *ivert    = (ui*)&buffer[offset];    offset+=countIndex*3;
    float *texcoord = (float*)&buffer[offset]; offset+=countVert*2;

    float *vert;
    float *norm;

    vId = new GLuint[countFrames];
    nId = new GLuint[countFrames];

    for(ui i=0;i<countFrames;i++)
    {
        vert = (float*)&buffer[offset]; offset+=countVert*3;
        norm = (float*)&buffer[offset]; offset+=countVert*3;

        glGenBuffersARB( 1, &vId[i] );
        glBindBufferARB(GL_ARRAY_BUFFER, vId[i] );
        glBufferDataARB(GL_ARRAY_BUFFER, sizeof(float)*countVert*3, vert, GL_STATIC_DRAW);
        glBindBufferARB(GL_ARRAY_BUFFER,0);

        glGenBuffersARB( 1, &nId[i] );
        glBindBufferARB(GL_ARRAY_BUFFER, nId[i] );
        glBufferDataARB(GL_ARRAY_BUFFER, sizeof(float)*countVert*3, norm, GL_STATIC_DRAW);
        glBindBufferARB(GL_ARRAY_BUFFER,0);

    }
    glGenBuffersARB( 1, &tcId );
    glBindBufferARB( GL_ARRAY_BUFFER, tcId );
    glBufferDataARB( GL_ARRAY_BUFFER, sizeof(float)*countVert*2, texcoord, GL_STATIC_DRAW );
    glBindBufferARB( GL_ARRAY_BUFFER, 0 );

    glGenBuffersARB( 1, &iId );
    glBindBufferARB(GL_ELEMENT_ARRAY_BUFFER, iId);
    glBufferDataARB(GL_ELEMENT_ARRAY_BUFFER, sizeof(ui)*countIndex*3, ivert, GL_STATIC_DRAW);
    glBindBufferARB(GL_ELEMENT_ARRAY_BUFFER, 0);

    iCount = countIndex*3;

    delete[] buffer;
}

void CModel::initializeGL()
{
    model_shader.addShaderFromSourceFile(QGLShader::Vertex,":/shaders/model.vert");
    model_shader.addShaderFromSourceFile(QGLShader::Fragment,":/shaders/model.frag");
    model_shader.link();
    model_shader.bind();
    model_shader.setUniformValue("text",0);
    vertexLocation = model_shader.attributeLocation("next_v");
    normalLocation = model_shader.attributeLocation("next_n");
    model_shader.release();
}

void CModel::setPosition(float *position)
{
    pos[0] = position[0];
    pos[1] = position[1];
    pos[2] = position[2];
}

void CModel::setPosition(float x, float y, float z)
{
    pos[0] = x;
    pos[1] = y;
    pos[2] = z;
}

void CModel::setScale(float x, float y, float z)
{
    scale[0] = x;
    scale[1] = y;
    scale[2] = z;
}

void CModel::setRotateParam(float angle, float x, float y, float z)
{
    rot[0] = angle;
    rot[1] = x;
    rot[2] = y;
    rot[3] = z;
}

void CModel::draw(float frame)
{
    int curr_frame = int(frame);
    int next_frame = curr_frame + 1;

    modelViewMatrix.setToIdentity();
    modelViewMatrix.translate(pos[0],pos[1],pos[2]);
    modelViewMatrix.rotate(rot[0],rot[1],rot[2],rot[3]);
    modelViewMatrix.scale(scale[0],scale[1],scale[2]);

    model_shader.bind();
    model_shader.setUniformValue("u_interp",frame-curr_frame);
    model_shader.setUniformValueArray("u_modelViewMatrix", &modelViewMatrix,1);

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, tId);

    glEnableClientState( GL_VERTEX_ARRAY );
    glEnableClientState(GL_NORMAL_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glBindBufferARB(GL_ARRAY_BUFFER, vId[curr_frame]);
    glVertexPointer( 3, GL_FLOAT, 0, NULL );
    glBindBufferARB( GL_ARRAY_BUFFER, nId[curr_frame]);
    glNormalPointer(GL_FLOAT, 0, NULL);
    glBindBufferARB( GL_ARRAY_BUFFER, tcId );
    glTexCoordPointer( 2, GL_FLOAT, 0, NULL );
    glBindBufferARB(GL_ELEMENT_ARRAY_BUFFER, iId);

    glBindBufferARB(GL_ARRAY_BUFFER, vId[next_frame]);
    glEnableVertexAttribArrayARB(vertexLocation);
    glVertexAttribPointerARB(vertexLocation, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glBindBufferARB(GL_ARRAY_BUFFER, nId[next_frame]);
    glEnableVertexAttribArrayARB(normalLocation);
    glVertexAttribPointerARB(normalLocation, 3, GL_FLOAT, GL_FALSE, 0, 0);

    glDrawElements(GL_TRIANGLES, iCount, GL_UNSIGNED_INT, NULL);

    glDisableVertexAttribArrayARB(vertexLocation);
    glDisableVertexAttribArrayARB(normalLocation);
    glBindBufferARB(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindBufferARB( GL_ARRAY_BUFFER, 0 );
    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_NORMAL_ARRAY);
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);

    glBindTexture(GL_TEXTURE_2D, 0);
    glDisable(GL_TEXTURE_2D);

    model_shader.release();
}
