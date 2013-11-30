#include "cforest.h"

#include "stdio.h"
#include "cnoise.h"
#include "clandscape.h"
#include "time.h"

#define TOP_BORDER 1.5f
#define BOT_BORDER 0.4f
CForest::CForest()
{
    iCountTrees = 0;
}

void CForest::draw()
{
    for(int i=0;i<iCountTrees;i++)
    {
        model.setPosition(trees[i][0], trees[i][1], trees[i][2]);
        model.draw();
    }
}

void CForest::initializeGL()
{
    model.initializeGL();
    model.loadFromFile("tree.3d", "tree.bmp");

    model.setScale(0.03f,0.03f,0.03f);
}

void CForest::generate(CLandscape *landscape)
{
    CNoise2D noise;

    noise.setCorrFunc(1.001f, 1.0f,-0.5f,-0.5f,0.5f,0.5f);
    noise.gen(time(NULL));

    for(float x=-50.0f;x<=50.0f;x+=0.25f)
        for(float y=-50.0f;y<=50.0f;y+=0.25f)
        {
            int chance = ((100.0f)/(TOP_BORDER-BOT_BORDER))*(noise.getHeight(x,y,10,1.5f,0.5)-BOT_BORDER);

            if(landscape->getSurfaceType(x,y) == 3 && rand()%100<chance)
            {
                trees[iCountTrees][0] = x;
                trees[iCountTrees][1] = y;
                trees[iCountTrees][2] = landscape->getHeight(x,y);
                iCountTrees++;
            }
        }
}
