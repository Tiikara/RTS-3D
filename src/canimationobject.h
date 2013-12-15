#ifndef CANIMATIONOBJECT_H
#define CANIMATIONOBJECT_H

#include <QHash>

#include "cbaseobject.h"

struct AnimationParam
{
    AnimationParam(int _transitionStart, int _loopStart, int _loopEnd, int _transitionEnd):
        transitionStart(_transitionStart),
        loopStart(_loopStart),
        loopEnd(_loopEnd),
        transitionEnd(_transitionEnd)
    {}

    int transitionStart;
    int loopStart;
    int loopEnd;
    int transitionEnd;
};

class CAnimationObject : public CBaseObject
{
public:
    CAnimationObject();

    void loadAnimationDesc(const char *fileName);
    void setAnimation(const QString &animationName);

    virtual void update();
    virtual void draw();
protected:
    QHash<QString,AnimationParam*> *animations;
private:
    float fCurrentFrame;
    AnimationParam *currAnimation;
    AnimationParam *nextAnimation;
};

#endif // CANIMATIONOBJECT_H
