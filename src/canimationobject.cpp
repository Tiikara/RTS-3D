#include "canimationobject.h"

#include <QFile>

CAnimationObject::CAnimationObject():
    fCurrentFrame(0.0f),
    currAnimation(NULL),
    nextAnimation(NULL)
{
}

void CAnimationObject::loadAnimationDesc(const char *fileName)
{
    QFile file(fileName);

    file.open(QIODevice::ReadOnly | QIODevice::Text);

    QTextStream stream(&file);

    QStringList line;

    while(!stream.atEnd())
    {
        line = stream.readLine().split(":");
        animations->insert(line[0],new AnimationParam(line[1].toInt(),line[2].toInt(),line[3].toInt(),line[4].toInt()));
    }
}

void CAnimationObject::setAnimation(const QString &animationName)
{
    nextAnimation = animations->value(animationName);

    if(currAnimation==NULL)
    {
        currAnimation = nextAnimation;
        fCurrentFrame = currAnimation->transitionStart;
    }
}

void CAnimationObject::update()
{
    fCurrentFrame += 0.1f;
    if(currAnimation==nextAnimation)
    {
        if((int)fCurrentFrame > currAnimation->loopEnd)
        {
            fCurrentFrame = currAnimation->loopEnd;
        }
        else
        if((int)fCurrentFrame == currAnimation->loopEnd)
        {
            fCurrentFrame = currAnimation->loopStart;
        }
    }
    else
    {
        if((int)fCurrentFrame > currAnimation->transitionEnd)
        {
            fCurrentFrame = currAnimation->transitionEnd;
        }
        else
        if((int)fCurrentFrame == currAnimation->transitionEnd)
        {
            fCurrentFrame = nextAnimation->transitionStart;
            currAnimation = nextAnimation;
        }
    }
}

void CAnimationObject::draw()
{
    model->draw(fCurrentFrame);
}
