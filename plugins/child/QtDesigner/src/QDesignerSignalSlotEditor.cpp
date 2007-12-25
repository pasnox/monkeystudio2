#include "QDesignerSignalSlotEditor.h"
#include "QtDesignerChild.h"

#include <QDesignerComponents>
#include <QDesignerFormEditorInterface>
#include <QDesignerFormWindowInterface>
#include <QDesignerFormWindowManagerInterface>

QDesignerSignalSlotEditor::QDesignerSignalSlotEditor( QtDesignerChild* p )
 : pDockWidget( 0 )
{
	// need core
	Q_ASSERT( p && p->core() );
	
	// dock title
	setWindowTitle( tr( "Signal/Slot Editor" ) );
	
	// object name
	setObjectName( "x-designer/signalsloteditor" );
	
	// create interface
	mInterface = QDesignerComponents::createSignalSlotEditor( p->core(), this );
	
	// set dock widget
	setWidget( mInterface );
}
