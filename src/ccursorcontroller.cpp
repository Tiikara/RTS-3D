#include "ccursorcontroller.h"
#include "MainWindow.h"

#include "cscene.h"

CCursorController::CCursorController()
{
    curCursor = &cursors[IdCursor::STANDART];
}

void CCursorController::initGL()
{
    cursors[IdCursor::STANDART].loadFromFile("");
    cursors[IdCursor::MOVE].loadFromFile("");
}

void CCursorController::setScene(CScene *scene)
{
    this->scene = scene;
}

void CCursorController::draw()
{
    QPoint cursorPos = MainWindow::getCursorPosition();
    curCursor->draw(cursorPos.x()-curCursor->getHeight(),cursorPos.y());
}

void CCursorController::update()
{
    CSelection *selection = scene->getSelection();
    int countSelection = selection->getCountSelected();

    if(countSelection==0)
        curCursor = &cursors[IdCursor::STANDART];

    curCursor = &cursors[ selection->getSelectedObjects()[0]->getIdCursor(NULL) ];
}
