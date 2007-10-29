#include "QDesignerActionEditor.h"
#include "QtDesignerChild.h"

#include <QDesignerComponents>
#include <QDesignerActionEditorInterface>

#include <QDesignerFormEditorInterface>
#include <QDesignerFormWindowInterface>
#include <QDesignerFormWindowManagerInterface>

QDesignerActionEditor::QDesignerActionEditor( QtDesignerChild* p )
 : QDockWidget( 0 )
{
	// need core
	Q_ASSERT( p && p->core() );
	
	// dock title
	setWindowTitle( tr( "Action Editor" ) );
	
	// object name
	setObjectName( "x-designer/actioneditor" );
	
	// create interface
	mInterface = QDesignerComponents::createActionEditor( p->core(), this );
	
	// track window changed
	connect( p->core()->formWindowManager(), SIGNAL( activeFormWindowChanged( QDesignerFormWindowInterface* ) ), mInterface, SLOT( setFormWindow( QDesignerFormWindowInterface* ) ) );
	
	// set dock widget
	setWidget( mInterface );
	
	// assign action editor to core
	p->core()->setActionEditor( mInterface );
}
