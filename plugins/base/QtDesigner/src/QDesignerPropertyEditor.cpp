#include "QDesignerPropertyEditor.h"
#include "QtDesignerChild.h"

#include <QDesignerComponents>
#include <QDesignerPropertyEditorInterface>
#include <QDesignerFormEditorInterface>
#include <QDesignerFormWindowManagerInterface>
#include <QDesignerFormWindowInterface>
#include <QDesignerFormWindowCursorInterface>

QDesignerPropertyEditor::QDesignerPropertyEditor( QtDesignerChild* p )
 : QDockWidget( 0 ), mForm( 0 )
{
	// need core
	Q_ASSERT( p && p->core() );
	
	// dock title
	setWindowTitle( tr( "Property Editor" ) );
	
	// object name 
	setObjectName( "x-designer/propertyeditor" );
	
	// create interface
	mInterface = QDesignerComponents::createPropertyEditor( p->core(), this );
	
	// track window changed
	connect( p->core()->formWindowManager(), SIGNAL( activeFormWindowChanged( QDesignerFormWindowInterface* ) ), this, SLOT( activeFormWindowChanged( QDesignerFormWindowInterface* ) ) );
	
	// et dock widget
	setWidget( mInterface );
	
	// assign property editor to core
	p->core()->setPropertyEditor( mInterface );
}

void QDesignerPropertyEditor::selectionChanged()
{
	// not need if no form
	if ( !mForm )
		return;
	
	// set current object for property editor
	mInterface->setObject( mForm->cursor()->hasSelection() ? mForm->cursor()->selectedWidget( 0 ) : mForm );
	
}

void QDesignerPropertyEditor::activeFormWindowChanged( QDesignerFormWindowInterface* w )
{
	// disconnect form selection changed
	if ( mForm )
		disconnect( mForm, SIGNAL( selectionChanged() ), this, SLOT( selectionChanged() ) );
	
	// set new object for property editor
	mForm = w;
	mInterface->setObject( mForm );
	
	// connect form selection changed
	if ( mForm )
		connect( mForm, SIGNAL( selectionChanged() ), this, SLOT( selectionChanged() ) );
}
