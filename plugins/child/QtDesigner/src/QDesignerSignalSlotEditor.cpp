/****************************************************************************
**
** 		Created using Monkey Studio v1.8.0.0
** Author    : Azevedo Filipe aka Nox P@sNox <pasnox@gmail.com>, The Monkey Studio Team
** Project   : Monkey Studio 2
** FileName  : QDesignerSignalSlotEditor.cpp
** Date      : 2007-11-04T22:53:17
** License   : GPL
** Comment   : Monkey Studio is a Free, Fast and lightweight crossplatform Qt RAD.
It's extendable with a powerfull plugins system.
** Home Page : http://www.monkeystudio.org
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/#include "QDesignerSignalSlotEditor.h"
#include "QtDesignerChild.h"

#include <QDesignerComponents>
#include <QDesignerFormEditorInterface>
#include <QDesignerFormWindowInterface>
#include <QDesignerFormWindowManagerInterface>

QDesignerSignalSlotEditor::QDesignerSignalSlotEditor( QtDesignerChild* p )
 : QDockWidget( 0 )
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
