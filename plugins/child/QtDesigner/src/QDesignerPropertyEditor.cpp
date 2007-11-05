/****************************************************************************
**
** 		Created using Monkey Studio v1.8.0.0
** Author    : Azevedo Filipe aka Nox P@sNox <pasnox@gmail.com>, The Monkey Studio Team
** Project   : Monkey Studio 2
** FileName  : QDesignerPropertyEditor.cpp
** Date      : 2007-11-04T22:53:16
** License   : GPL
** Comment   : Monkey Studio is a Free, Fast and lightweight crossplatform Qt RAD.
It's extendable with a powerfull plugins system.
** Home Page : http://www.monkeystudio.org
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/
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
