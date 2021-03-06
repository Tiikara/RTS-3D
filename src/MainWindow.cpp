#include "MainWindow.h"

#include <QTimer>

MainWindow *mainWindow;

MainWindow::MainWindow(QWidget *parent)
    : QGLWidget(parent)
{
    mainWindow = this;

    setMouseTracking(true);

    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(updateGL()));
    connect(timer, SIGNAL(timeout()), this, SLOT(updateLogic()));
    timer->start(16);
}

float MainWindow::getWidth()
{
    return mainWindow->width();
}

float MainWindow::getHeight()
{
    return mainWindow->height();
}

QPoint MainWindow::getCursorPosition()
{
    return QPoint(mainWindow->mouseEvent.x(),mainWindow->mouseEvent.y());
}

void MainWindow::initializeGL()
{
    initExtensions();
    glClearColor(0.2,0.6,0.9,1.0);
    glEnable(GL_DEPTH_TEST);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    scene.initializeGL();
}

 void MainWindow::mouseMoveEvent(QMouseEvent *me)
 {
     mouseEvent.fx = me->x();
     mouseEvent.fy = height() - me->y();
     mouseEvent.button = me->buttons();

     scene.mouseMoveEvent(&mouseEvent);
 }

 void MainWindow::mousePostitionEvent()
 {
     QPoint positionMouse = mapFromGlobal( QCursor::pos());
     positionMouse.setY(height()-positionMouse.y());
     scene.mousePositionEvent(&positionMouse);
 }

 void MainWindow::wheelEvent(QWheelEvent *we)
 {
     scene.wheelEvent(we);
 }
 void MainWindow::keyPressEvent(QKeyEvent *k)
 {
    scene.keyPressEvent(k);
 }

 void MainWindow::keyReleaseEvent(QKeyEvent *k)
 {
    scene.keyReleaseEvent(k);
 }

void MainWindow::resizeGL(int w, int h)
{
    glViewport(0,0,w,h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60.0,(float)w/(float)h,0.005,100.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}
void MainWindow::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    scene.draw();
}

void MainWindow::mousePressEvent(QMouseEvent *me)
{
    mouseEvent.fx = me->x();
    mouseEvent.fy = height() - me->y();
    mouseEvent.button = me->buttons();

    scene.mousePressEvent(&mouseEvent);
}

void MainWindow::mouseReleaseEvent(QMouseEvent *me)
{
    mouseEvent.fx = me->x();
    mouseEvent.fy = height() - me->y();
    mouseEvent.button = me->buttons();

    scene.mouseReleaseEvent(&mouseEvent);
}

void MainWindow::updateLogic()
{
    mousePostitionEvent();

    scene.update();
}

MainWindow::~MainWindow()
{
    
}
