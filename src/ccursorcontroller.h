#ifndef CCURSORCONTROLLER_H
#define CCURSORCONTROLLER_H

#include "ctexture.h"

class CScene;

enum IdCursor
{
    STANDART,
    MOVE,
    COUNT
};

class CCursorController
{
public:
    CCursorController();

    void initGL();

    void setScene(CScene *scene);

    void draw();
    void update();
private:
    CScene *scene;

    CTexture cursors[IdCursor::COUNT];
    CTexture *curCursor;

};

#endif // CCURSORCONTROLLER_H
