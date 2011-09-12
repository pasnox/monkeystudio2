#ifndef XUPPAGEEDITOR_H
#define XUPPAGEEDITOR_H

#include "MonkeyExport.h"

#include <QFrame>

class XUPProjectItem;

class Q_MONKEY_EXPORT XUPPageEditor : public QFrame
{
    Q_OBJECT
    
public:
    XUPPageEditor( QWidget* parent = 0 );
    virtual ~XUPPageEditor();
    
    virtual void setup( XUPProjectItem* project ) = 0;
    virtual void finalize() = 0;
};

#endif // XUPPAGEEDITOR_H
