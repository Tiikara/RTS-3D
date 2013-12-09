#include "generateforest.h"

#include "stdio.h"
#include "cnoise.h"
#include "clandscape.h"
#include "time.h"

#define TOP_BORDER 1.5f
#define BOT_BORDER 0.4f

void generateForests(CTree **treesOut, int *countOut, CLandscape *landscape)
{
    CNoise2D noise;

    noise.setCorrFunc(1.001f, 1.0f,-0.5f,-0.5f,0.5f,0.5f);
    noise.gen(time(NULL));

    float height;
    int chance;

    for(float x=-50.0f;x<=50.0f;x+=0.25f)
        for(float y=-50.0f;y<=50.0f;y+=0.25f)
        {
            height = landscape->getHeight(x,y);

            chance = ((100.0f)/(TOP_BORDER-BOT_BORDER))*(noise.getHeight(x,y,10,1.5f,0.5)-BOT_BORDER);

            if(landscape->getSurfaceType(height) == 3 && rand()%100<chance)
            {
                treesOut[*countOut] = new CTree;
                treesOut[*countOut]->setPosition(x,y,height);
                (*countOut)++;
            }
        }
}
