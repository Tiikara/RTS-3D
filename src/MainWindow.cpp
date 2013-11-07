#include "MainWindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QGLWidget(parent)
{
    setMouseTracking(true);

    camera.setLandscape(&landscape);
}
void MainWindow::initializeGL()
{
    initExtensions();
    glClearColor(0.2,0.6,0.9,1.0);
    glEnable(GL_DEPTH_TEST);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    landscape.loadSettings("landscape.ini");
    landscape.initializeGL();
}

 void MainWindow::mouseMoveEvent(QMouseEvent *me)
 {

 }

 void MainWindow::wheelEvent(QWheelEvent *we)
 {
     camera.wheelEvent(we);
     updateGL();
 }
 void MainWindow::keyPressEvent(QKeyEvent *k)
 {
     if(k->key() == Qt::Key_Space)
         landscape.generateLandscape();

     camera.keyPressEvent(k);
     updateGL();
 }

void MainWindow::resizeGL(int w, int h)
{
    glViewport(0,0,w,h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60.0,(float)w/(float)h,0.0005,30.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}
void MainWindow::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    camera.look();

    landscape.draw(camera.getPosition());
}

MainWindow::~MainWindow()
{
    
}
