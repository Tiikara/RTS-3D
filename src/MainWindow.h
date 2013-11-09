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
#include "ccamera.h"

#include "clandscape.h"
#include "cmodel.h"

class MainWindow : public QGLWidget
{
    Q_OBJECT

private:

    CLandscape landscape;
    CCamera camera;

    CModel model;
protected:
    void initializeGL();
    void genTexture();
    void resizeGL(int w, int h);
    void paintGL();
    void mouseMoveEvent(QMouseEvent *);
    void wheelEvent(QWheelEvent *we);
    void keyPressEvent(QKeyEvent *);
    void keyReleaseEvent(QKeyEvent *);
protected slots:
    void updateLogic();
public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();
};

#endif // MAINWINDOW_H
