#ifndef $Class Name.upper$_H
#define $Class Name.upper$_H

#include <$Qt Widget Class$>

class Ui_$Class Name$;
class QEvent;

class $Class Name$ : public $Qt Widget Class$
{
    Q_OBJECT

public:
    $Class Name$( QWidget* parent = 0 );
    virtual ~$Class Name$();

public slots:
    virtual void retranslateUi();
    
protected:
    Ui_$Class Name$* ui;
    
    virtual void changeEvent( QEvent* event );
};

#endif // $Class Name.upper$_H
