#include "QDesignerObjectInspector.h"
#include "QtDesignerChild.h"

#include <QDesignerComponents>
#include <QDesignerObjectInspectorInterface>
#include <QDesignerFormEditorInterface>
#include <QDesignerFormWindowManagerInterface>
#include <QDesignerFormWindowInterface>

QDesignerObjectInspector::QDesignerObjectInspector( QtDesignerChild* p )
 : pDockWidget( 0 )
{
	// need core
	Q_ASSERT( p && p->core() );
	
	// dock title
	setWindowTitle( tr( "Object Inspector" ) );
	
	// object name
	setObjectName( "x-designer/objectinspector" );
	
	// create interface
	mInterface = QDesignerComponents::createObjectInspector( p->core(), this );
	
	// set dock widget
	setWidget( mInterface );
	
	// assign object inspector to core
	p->core()->setObjectInspector( mInterface );
}
