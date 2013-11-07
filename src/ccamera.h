#ifndef CAMERA_H
#define CAMERA_H

#include "GL/glu.h"
#include "gl_ext/libExt.h"
#include "clandscape.h"

typedef float vec3f[3];
typedef float* pvec3f;

class CCamera
{
public:
    CCamera();

    void look();

    void setLandscape(CLandscape *_landscape);

    pvec3f getPosition();

    void keyPressEvent(QKeyEvent *k);
    void mouseMoveEvent(QMouseEvent *me);
    void wheelEvent(QWheelEvent *we);
private:

    void updateAngle();

    CLandscape *landscape;

    vec3f pos;
    vec3f vdir;

    float max_va;
    float min_va;
    float curr_va;

    float max_h;
    float min_h;
    float curr_h;

    float rotate;


};

#endif // CAMERA_H
