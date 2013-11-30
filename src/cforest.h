#ifndef CFOREST_H
#define CFOREST_H

#include <GL/gl.h>
#include "cmodel.h"

typedef float vec3f[3];

class CLandscape;

class CForest
{
public:
    CForest();
    CModel model;
    void draw();

    void initializeGL();
    void generate(CLandscape *landscape);
private:
    int iCountTrees;
    vec3f trees[100000];


};

#endif // CFOREST_H
