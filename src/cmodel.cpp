#include "cmodel.h"
#include <stdio.h>

#include <QGLContext>
#include <QImage>
typedef unsigned int ui;

CModel::CModel()
{
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

    ui offset = 13;
    ui    *ivert    = (ui*)&buffer[offset]; offset+=countIndex*3;
    float *texcoord = (float*)&buffer[offset]; offset+=countVert*2;
    float *vert     = (float*)&buffer[offset]; offset+=countVert*3;
    float *norm     = (float*)&buffer[offset];

    glGenBuffersARB( 1, &tcId );
    glBindBufferARB( GL_ARRAY_BUFFER, tcId );
    glBufferDataARB( GL_ARRAY_BUFFER, sizeof(float)*countVert*2, texcoord, GL_STATIC_DRAW );
    glBindBufferARB( GL_ARRAY_BUFFER, 0 );

    glGenBuffersARB( 1, &iId );
    glBindBufferARB(GL_ELEMENT_ARRAY_BUFFER, iId);
    glBufferDataARB(GL_ELEMENT_ARRAY_BUFFER, sizeof(ui)*countIndex*3, ivert, GL_STATIC_DRAW);
    glBindBufferARB(GL_ELEMENT_ARRAY_BUFFER, 0);

    glGenBuffersARB( 1, &nId );
    glBindBufferARB(GL_ARRAY_BUFFER, nId );
    glBufferDataARB(GL_ARRAY_BUFFER, sizeof(float)*countVert*3, norm, GL_STATIC_DRAW);
    glBindBufferARB(GL_ARRAY_BUFFER,0);

    glGenBuffersARB( 1, &vId );
    glBindBufferARB(GL_ARRAY_BUFFER, vId );
    glBufferDataARB(GL_ARRAY_BUFFER, sizeof(float)*countVert*3, vert, GL_STATIC_DRAW);
    glBindBufferARB(GL_ARRAY_BUFFER,0);

    iCount = countIndex*3;

    return;
}

void CModel::draw()
{
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, tId);

    glEnableClientState( GL_VERTEX_ARRAY );
    glEnableClientState(GL_NORMAL_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glBindBufferARB(GL_ARRAY_BUFFER, vId);
    glVertexPointer( 3, GL_FLOAT, 0, NULL );
    glBindBufferARB( GL_ARRAY_BUFFER, nId );
    glNormalPointer(GL_FLOAT, 0, NULL);
    glBindBufferARB( GL_ARRAY_BUFFER, tcId );
    glTexCoordPointer( 2, GL_FLOAT, 0, NULL );
    glBindBufferARB(GL_ELEMENT_ARRAY_BUFFER, iId);
    glDrawElements(GL_TRIANGLES, iCount, GL_UNSIGNED_INT, NULL);
    glBindBufferARB(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindBufferARB( GL_ARRAY_BUFFER, 0 );
    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_NORMAL_ARRAY);
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);

    glBindTexture(GL_TEXTURE_2D, 0);
    glDisable(GL_TEXTURE_2D);
}
