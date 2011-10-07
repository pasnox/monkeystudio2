#include "$Class Name$.h"

#include <QEvent>

$Class Name$::$Class Name$( QWidget* parent )
    : $Qt Widget Class$( parent )
{
    setupUi( this );
}

$Class Name$::~$Class Name$()
{
}

void $Class Name$::retranslateUi()
{
    Ui_$Class Name$::retranslateUi( this );
    // do your custom retranslate here
}

void $Class Name$::changeEvent( QEvent* event )
{
    $Qt Widget Class$::changeEvent( event );
    
    if ( event->type() == QEvent::LanguageChange ) {
        retranslateUi();
    }
}
