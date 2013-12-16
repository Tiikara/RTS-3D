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

struct AnimationList
{
    void loadAnimationDesc(const char *fileName);
    QHash<QString,AnimationParam*> animations;
};

class CAnimationObject : public CBaseObject
{
public:
    CAnimationObject(IdObject id, float radius, CModel *model, AnimationList *animations);

    void loadAnimationDesc(const char *fileName);
    void setAnimation(const QString &animationName);

    virtual void update();
    virtual void draw();
protected:
    AnimationList *animList;
private:
    float fCurrentFrame;
    AnimationParam *currAnimation;
    AnimationParam *nextAnimation;
};

#endif // CANIMATIONOBJECT_H
