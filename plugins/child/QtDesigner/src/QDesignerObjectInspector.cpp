/****************************************************************************
**
** 		Created using Monkey Studio v1.8.0.0
** Author    : Azevedo Filipe aka Nox P@sNox <pasnox@gmail.com>, The Monkey Studio Team
** Project   : Monkey Studio 2
** FileName  : QDesignerObjectInspector.cpp
** Date      : 2007-11-04T22:53:15
** License   : GPL
** Comment   : Monkey Studio is a Free, Fast and lightweight crossplatform Qt RAD.
It's extendable with a powerfull plugins system.
** Home Page : http://www.monkeystudio.org
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/
#include "QDesignerObjectInspector.h"
#include "QtDesignerChild.h"

#include <QDesignerComponents>
#include <QDesignerObjectInspectorInterface>
#include <QDesignerFormEditorInterface>
#include <QDesignerFormWindowManagerInterface>
#include <QDesignerFormWindowInterface>

QDesignerObjectInspector::QDesignerObjectInspector( QtDesignerChild* p )
 : QDockWidget( 0 )
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
