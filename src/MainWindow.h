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

#include "cscene.h"

class MainWindow : public QGLWidget
{
    Q_OBJECT

private:
    CScene scene;
protected:
    void initializeGL();
    void genTexture();
    void resizeGL(int w, int h);
    void paintGL();
    void mousePressEvent(QMouseEvent *);
    void mouseReleaseEvent(QMouseEvent *);
    void mouseMoveEvent(QMouseEvent *);
    void mousePostitionEvent();
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
