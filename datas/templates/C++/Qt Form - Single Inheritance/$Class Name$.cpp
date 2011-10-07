#include "$Class Name$.h"
#include "ui_$Class Name$.h"

#include <QEvent>

$Class Name$::$Class Name$( QWidget* parent )
    : $Qt Widget Class$( parent ), ui( new Ui_$Class Name$ )
{
    ui->setupUi( this );
}

$Class Name$::~$Class Name$()
{
    delete ui;
}

void $Class Name$::retranslateUi()
{
    ui->retranslateUi( this );
    // do your custom retranslate here
}

void $Class Name$::changeEvent( QEvent* event )
{
    $Qt Widget Class$::changeEvent( event );
    
    if ( event->type() == QEvent::LanguageChange ) {
        retranslateUi();
    }
}
