#include "$Class Name$.h"
#include "ui_$Class Name$.h"

$Class Name$::$Class Name$( QWidget* parent )
    : XUPPageEditor( parent )
{
    ui = new Ui_$Class Name$;
    ui->setupUi( this );
    mProject = 0;
}

$Class Name$::~$Class Name$()
{
    delete ui;
}

void $Class Name$::setup( XUPProjectItem* project )
{
}

void $Class Name$::finalize()
{
}
