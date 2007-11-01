#include "QDesignerPropertyEditor.h"
#include "QtDesignerChild.h"

#include <QDesignerComponents>
#include <QDesignerPropertyEditorInterface>
#include <QDesignerFormEditorInterface>
#include <QDesignerFormWindowManagerInterface>
#include <QDesignerFormWindowInterface>
#include <QDesignerFormWindowCursorInterface>

QDesignerPropertyEditor::QDesignerPropertyEditor( QtDesignerChild* p )
 : QDockWidget( 0 )
{
	// need core
	Q_ASSERT( p && p->core() );
	
	// dock title
	setWindowTitle( tr( "Property Editor" ) );
	
	// object name 
	setObjectName( "x-designer/propertyeditor" );
	
	// create interface
	mInterface = QDesignerComponents::createPropertyEditor( p->core(), this );
	
	// et dock widget
	setWidget( mInterface );
	
	// assign property editor to core
	p->core()->setPropertyEditor( mInterface );
}
