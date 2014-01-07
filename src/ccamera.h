#ifndef CAMERA_H
#define CAMERA_H

// Qt 4.7.4
#include <QGLShaderProgram>
#include <QGLWidget>
//---

#include "GL/glu.h"
#include "gl_ext/libExt.h"
#include "clandscape.h"

#include "cmouseevent.h"

typedef float vec3f[3];
typedef float* pvec3f;

class CCamera
{
public:
    CCamera();

    void look();
    void update();

    void setLandscape(CLandscape *_landscape);

    pvec3f getPosition();
    float getRotate() { return rotate; }
    float getAngle() { return curr_va; }

    void keyPressEvent(QKeyEvent *k);
    void keyReleaseEvent(QKeyEvent *k);
    void mouseMoveEvent(CMouseEvent *me);
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

    float moveSpeed;
    float speedScroll;

    bool pressedKeys[4];

    float old_mx;
};

#endif // CAMERA_H
