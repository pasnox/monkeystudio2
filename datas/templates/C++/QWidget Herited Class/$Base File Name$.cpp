#include "$Base File Name$.h"

#include <QEvent>

$Class Name$::$Class Name$( QWidget* parent )
    : $Parent Class$( parent )
{
}

$Class Name$::~$Class Name$()
{
}

void $Class Name$::retranslateUi()
{
    // do your custom retranslate here
}

void $Class Name$::changeEvent( QEvent* event )
{
    $Parent Class$::changeEvent( event );
    
    if ( event->type() == QEvent::LanguageChange ) {
        retranslateUi();
    }
}
