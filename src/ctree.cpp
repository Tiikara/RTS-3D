#include "ctree.h"

CModel *CTree::model;

CTree::CTree()
{
}

void CTree::draw()
{
    model->setPosition(pos);
    model->draw();
}

void CTree::setPosition(float x, float y, float z)
{
    pos[0] = x;
    pos[1] = y;
    pos[2] = z;
}

float *CTree::getPosition()
{
    return pos;
}

float CTree::getRadiusObject()
{
    return 0.5f;
}

void CTree::initializeGLModel()
{
    model = new CModel;
    model->initializeGL();
    model->loadFromFile("tree.3d", "tree.bmp");

    model->setScale(0.03f,0.03f,0.03f);
}
