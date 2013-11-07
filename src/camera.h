#ifndef CAMERA_H
#define CAMERA_H

#include "GL/glu.h"
#include "gl_ext/libExt.h"
#include "landscape.h"

typedef float vec3f[3];

class Camera
{
public:
    Camera();

    void look();

    void setLandscape(Landscape *_landscape);

    float getPositionX();
    float getPositionY();
    float getPositionZ();

    void keyPressEvent(QKeyEvent *k);
    void mouseMoveEvent(QMouseEvent *me);
    void wheelEvent(QWheelEvent *we);
private:

    void updateAngle();

    Landscape *landscape;

    float posX;
    float posY;

    float rotate;

    float max_va;
    float min_va;
    float curr_va;

    float max_h;
    float min_h;
    float curr_h;

    vec3f v;
    vec3f e;
};

#endif // CAMERA_H
