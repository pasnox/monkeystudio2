#ifndef $Base File Name.upper$_H
#define $Base File Name.upper$_H

#include <$Parent Class$>

class QEvent;

class $Class Name$ : $Herits Type$ $Parent Class$
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

#endif // $Base File Name.upper$_H
