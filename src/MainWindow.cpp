#include "MainWindow.h"

#include <QTimer>

MainWindow::MainWindow(QWidget *parent)
    : QGLWidget(parent)
{
    setMouseTracking(true);

    camera.setLandscape(&landscape);

    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(updateGL()));
    connect(timer, SIGNAL(timeout()), this, SLOT(updateLogic()));
    timer->start(16);
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
 }
 void MainWindow::keyPressEvent(QKeyEvent *k)
 {
     if(k->key() == Qt::Key_Space)
         landscape.generateLandscape();

     camera.keyPressEvent(k);
 }

 void MainWindow::keyReleaseEvent(QKeyEvent *k)
 {
    camera.keyReleaseEvent(k);
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

void MainWindow::updateLogic()
{
    camera.update();
}

MainWindow::~MainWindow()
{
    
}
