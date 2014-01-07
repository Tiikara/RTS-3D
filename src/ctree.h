#ifndef CTREE_H
#define CTREE_H

#include "cbaseobject.h"
#include "cmodel.h"

class CTree
{
public:
    CTree();

    void draw();

    void setPosition(float x, float y, float z);
    float *getPosition();

    float getRadiusObject();

    static void initializeGLModel();
private:
    static CModel *model;
    int health;

    float pos[3];
};

#endif // CTREE_H
