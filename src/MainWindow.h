#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QGLShaderProgram>
#include <QGLWidget>
#include <QString>
#include "GL/glu.h"
#include "gl_ext/libExt.h"
#include "math.h"
#include <QMouseEvent>
#include <QKeyEvent>
#include "camera.h"

#include "landscape.h"

class MainWindow : public QGLWidget
{
private:

    Landscape landscape;

    Camera camera;
protected:
    void initializeGL();
    void genTexture();
    void resizeGL(int w, int h);
    void paintGL();
    void mouseMoveEvent(QMouseEvent *);
    void wheelEvent(QWheelEvent *we);
    void keyPressEvent(QKeyEvent *);
public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();
};

#endif // MAINWINDOW_H
