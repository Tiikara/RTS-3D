#ifndef CCOMMANDVIEWCONTROLLER_H
#define CCOMMANDVIEWCONTROLLER_H

#include "cwindow.h"
#include "cbutton.h"
#include "cselection.h"
#include "cbaseobject.h"

class CCommandViewController : public  QObject
{
    Q_OBJECT

public:
    CCommandViewController();

    void initializeGL();

    void setSelection(CSelection *selection);

    void drawInterface();
    void updateInterface();

    void mouseMoveEvent(QMouseEvent *me);
    void mousePressEvent(QMouseEvent *me);
    void mouseReleaseEvent(QMouseEvent *me);
    void mousePositionEvent(QPoint *p);
private:
    CSelection *selection;

    CWindow baseInterface;
    CButton butAttack;
    CButton butDef;

    CWindow peasantIcon;

protected slots:
    void commandDefense();
    void commandAttack();
};

#endif // CCOMMANDVIEWCONTROLLER_H
