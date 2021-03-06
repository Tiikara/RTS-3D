#ifndef CWINDOW_H
#define CWINDOW_H

#include <QtOpenGL>
#include <QObject>
#include <QMouseEvent>
#include <QList>
#include "ctexture.h"

typedef float vec2f[2];

class CWindow : public QObject
{
    Q_OBJECT

public:
    CWindow();
    ~CWindow();

    void setPosition(float x, float y);
    float *getPosition();
    void setWindowSize(float width, float height);
    void getWindowSize(float *width, float *height);
    void setPriority(int priority);

    int getPriority() { return priority; }

    void loadTexture(const char *fileName);

    virtual void draw();
    virtual void update();

    bool mousePressEvent(float mouseX, float mouseY, bool leftMouse, bool rightMouse);
    bool mouseReleaseEvent(float mouseX, float mouseY, bool leftMouse, bool rightMouse);
    bool mouseMoveEvent(float mouseX, float mouseY, bool leftMouse, bool rightMouse);
    bool mousePositionEvent(float mouseX, float mouseY);

    void addChild( CWindow *window, float posX, float posY, int priority = 0 );
    void clearAllChilds();
protected:
    virtual bool pressEvent(float mouseX, float mouseY, bool leftMouse, bool rightMouse);
    virtual bool releaseEvent(float mouseX, float mouseY, bool leftMouse, bool rightMouse);
    virtual bool moveEvent(float mouseX, float mouseY, bool leftMouse, bool rightMouse);
    virtual bool positionEvent(float mouseX, float mouseY);
private:
    void transformCoordToLocal(float *mouseX, float *mouseY);
    bool notInWindowMouse(float mouseX, float mouseY);

    vec2f pos;
    float width;
    float height;

    int priority;

    QList<CWindow*> childs;

    CTexture *tex;
};

#endif // CWINDOW_H
