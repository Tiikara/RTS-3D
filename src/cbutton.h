#ifndef CBUTTON_H
#define CBUTTON_H

#include "cwindow.h"
#include <QGLShaderProgram>

class CButton : public CWindow
{
    Q_OBJECT

public:
    CButton();

    void loadTextures(const char *normalTex, const char *selectedTex, const char *pressedTex);

    virtual void draw();
    virtual void update();

protected:
    virtual bool pressEvent(float mouseX, float mouseY, bool leftMouse, bool rightMouse);
    virtual bool releaseEvent(float mouseX, float mouseY, bool leftMouse, bool rightMouse);
    virtual bool moveEvent(float mouseX, float mouseY, bool leftMouse, bool rightMouse);
    virtual bool positionEvent(float mouseX, float mouseY);
private:
    bool selectedbuf;
    bool pressedbuf;

    bool selected;
    bool pressed;

    bool active;

    GLuint normalId;
    GLuint pressedId;
    GLuint selectedId;
signals:
    void clicked();
};

#endif // CBUTTON_H
