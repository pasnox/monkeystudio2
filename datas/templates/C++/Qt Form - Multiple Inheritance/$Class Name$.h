#ifndef $Class Name.upper$_H
#define $Class Name.upper$_H

#include "ui_$Class Name$.h"

class QEvent;

class $Class Name$ : public $Qt Widget Class$, public Ui_$Class Name$
{
    Q_OBJECT

public:
    $Class Name$( QWidget* parent = 0 );
    virtual ~$Class Name$();

public slots:
    virtual void retranslateUi();
    
protected:
    virtual void changeEvent( QEvent* event );
};

#endif // $Class Name.upper$_H
