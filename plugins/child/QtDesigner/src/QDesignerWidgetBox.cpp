#include "QDesignerWidgetBox.h"
#include "QtDesignerChild.h"

#include <QDir>
#include <QDesignerComponents>
#include <QDesignerWidgetBoxInterface>
#include <QDesignerFormEditorInterface>

QDesignerWidgetBox::QDesignerWidgetBox( QtDesignerChild* p )
 : pDockWidget( 0 )
{
	// need core
	Q_ASSERT( p && p->core() );
	
	// set dock title
	setWindowTitle( tr( "Widget Box" ) );
	
	// object name
	setObjectName( "x-designer/widgetbox" );
	
	// create widget box interface
	mInterface = QDesignerComponents::createWidgetBox( p->core(), this );
	
	// load defaults widgets
	mInterface->setFileName( ":/trolltech/widgetbox/widgetbox.xml" );
	mInterface->load();
	
	// laod user widgets
	mInterface->setFileName( QDir::homePath().append( "/designer/widgetbox.xml" ) );
	mInterface->load();
	
	// set widget for dock
	setWidget( mInterface );
	
	// assign widget box for core
	p->core()->setWidgetBox( mInterface );
}
