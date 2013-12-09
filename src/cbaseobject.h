#ifndef CBASEOBJECT_H
#define CBASEOBJECT_H

typedef float vec3f[3];

class CBaseObject
{
public:
    CBaseObject();

    float *getPosition();
    float getRadiusObject();
    void setPosition(float *position);
    void setPosition(float x, float y, float z);

    virtual void draw(){}
    virtual void update(){}
protected:
    int id;

    vec3f pos;
    float radius;
};

#endif // CBASEOBJECT_H
