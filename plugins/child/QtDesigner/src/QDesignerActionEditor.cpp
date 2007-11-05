/****************************************************************************
**
** 		Created using Monkey Studio v1.8.0.0
** Author    : Azevedo Filipe aka Nox P@sNox <pasnox@gmail.com>, The Monkey Studio Team
** Project   : Monkey Studio 2
** FileName  : QDesignerActionEditor.cpp
** Date      : 2007-11-04T22:53:13
** License   : GPL
** Comment   : Monkey Studio is a Free, Fast and lightweight crossplatform Qt RAD.
It's extendable with a powerfull plugins system.
** Home Page : http://www.monkeystudio.org
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/
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
	
	// set dock widget
	setWidget( mInterface );
	
	// assign action editor to core
	p->core()->setActionEditor( mInterface );
}
