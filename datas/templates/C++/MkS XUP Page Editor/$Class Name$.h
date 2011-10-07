#ifndef $Class Name.upper$_H
#define $Class Name.upper$_H

#include "xupmanager/gui/XUPPageEditor.h"

class Ui_$Class Name$;

class $Class Name$ : public XUPPageEditor
{
    Q_OBJECT

public:
    $Class Name$( QWidget* parent = 0 );
    virtual ~$Class Name$();

    virtual void setup( XUPProjectItem* project );
    virtual void finalize();

protected:
    Ui_$Class Name$* ui;
    XUPProjectItem* mProject;
};

#endif // $Class Name.upper$_H
