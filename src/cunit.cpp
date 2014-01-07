#include "cunit.h"

#include "cscene.h"

CUnit::CUnit(IdObject _id, float _radius, CModel *_model, AnimationList *_animations):
    CAnimationObject(_id,_radius,_model,_animations),
    currState(NULL),
    nextState(NULL)
{
}

void CUnit::setState(IState *state)
{
    nextState = state;

    if(currState==NULL)
    {
        currState = nextState;
        currState->start();
    }

    if(currState==nextState)
        currState->start();
}

void CUnit::update()
{
    CAnimationObject::update();

    CScene *scene = CScene::sharedScene();
    CLandscape *landscape = scene->getLandscape();

    pos[2] = landscape->getHeight(pos[0],pos[1]);

    if(currState==NULL)
        return;

    if(currState!=nextState)
    {
        if(currState->end())
        {
            currState = nextState;
            currState->start();
        }
    }
    else
    {
       currState->update();
    }
}

void StateMove::start()
{
    _this->setAnimation("ходьба");
}

void StateMove::update()
{
    float transPos[2] = { newPos[0] - _this->pos[0] , newPos[1] - _this->pos[1] };

    float lengthOne = 1.0f/sqrtf(transPos[0]*transPos[0]+transPos[1]*transPos[1]);
    float dirMove[2] = {lengthOne * transPos[0] * 0.1f,
                        lengthOne * transPos[1] * 0.1f};

    float angleOffsetDir = 0.0f;
    bool findDir = true;

    float dirMoveRot[2];

    while(angleOffsetDir < 90.0f && findDir)
    {
        float angle[2] = {angleOffsetDir,-angleOffsetDir};
        float tmpDirMoveRot[2][2];

        for(int i=0;i<2;i++)
        {
            tmpDirMoveRot[i][0] = dirMove[0] * cos(angle[i]) - dirMove[1] * sin(angle[i]);
            tmpDirMoveRot[i][1] = dirMove[0] * sin(angle[i]) + dirMove[1] * cos(angle[i]);
        }

        float distance[2];
        float imp[2];
        for(int i=0;i<2;i++)
        {
            imp[0] = newPos[0]-(tmpDirMoveRot[i][0]+_this->pos[0]);
            imp[1] = newPos[1]-(tmpDirMoveRot[i][1]+_this->pos[1]);
            distance[i] = sqrtf(imp[0]*imp[0]+imp[1]*imp[1]);
        }


        bool region[2] = { false };

        for(int i=0;i<2;i++)
        {
            if(CScene::sharedScene()->
                    isFreeRegion(_this->pos[0]+tmpDirMoveRot[i][0], _this->pos[1]+tmpDirMoveRot[i][1],_this->radius,_this))
            {
                region[i] = true;
                findDir = false;
            }
        }

        if(distance[0] <= distance[1])
        {
            if(region[0])
            {
                dirMoveRot[0] = tmpDirMoveRot[0][0];
                dirMoveRot[1] = tmpDirMoveRot[0][1];
            }
            else if(region[1])
            {
                dirMoveRot[0] = tmpDirMoveRot[1][0];
                dirMoveRot[1] = tmpDirMoveRot[1][1];
            }
        }
        else
        {
            if(region[1])
            {
                dirMoveRot[0] = tmpDirMoveRot[1][0];
                dirMoveRot[1] = tmpDirMoveRot[1][1];
            }
            else if(region[0])
            {
                dirMoveRot[0] = tmpDirMoveRot[0][0];
                dirMoveRot[1] = tmpDirMoveRot[0][1];
            }
        }



        angleOffsetDir += 0.01f;
    }

    if(findDir)
    {
        _this->setState(nextState);
        return;
    }

    _this->updateSmoothRotateTo(_this->pos[0]+dirMoveRot[0],_this->pos[1]+dirMoveRot[1], 15.0f);

    _this->pos[0]+=dirMoveRot[0];
    _this->pos[1]+=dirMoveRot[1];

    if(fabsf(_this->pos[0] - newPos[0])<=fabsf(dirMoveRot[0]) &&
       fabsf(_this->pos[1] - newPos[1])<=fabsf(dirMoveRot[1]))
    {
        _this->setState(nextState);
        _this->pos[0]=newPos[0];
        _this->pos[1]=newPos[1];
    }
}

void StateStay::start()
{
    _this->setAnimation("базовое стояние");
}
