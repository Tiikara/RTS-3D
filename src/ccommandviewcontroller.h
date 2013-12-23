#ifndef CCOMMANDVIEWCONTROLLER_H
#define CCOMMANDVIEWCONTROLLER_H

#include "cwindow.h"
#include "cbutton.h"
#include "cselection.h"
#include "cbaseobject.h"
#include "clandscape.h"

#include "cmouseevent.h"

class CCommandViewController : public  QObject
{
    Q_OBJECT

public:
    CCommandViewController();

    void initializeGL();

    void setSelection(CSelection *selection);
    void setLandscape(CLandscape *landscape);

    void drawInterface();
    void updateInterface();

    void mouseMoveEvent(CMouseEvent *me);
    void mousePressEvent(CMouseEvent *me);
    void mouseReleaseEvent(CMouseEvent *me);
    void mousePositionEvent(QPoint *p);
private:
    CSelection *selection;
    CLandscape *landscape;

    CWindow baseInterface;
    CButton butAttack;
    CButton butDef;

    CWindow peasantIcon;

protected slots:
    void commandDefense();
    void commandAttack();
};

#endif // CCOMMANDVIEWCONTROLLER_H
