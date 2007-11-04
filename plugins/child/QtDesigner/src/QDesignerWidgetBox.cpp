/****************************************************************************
**
** 		Created using Monkey Studio v1.8.0.0
** Author    : Azevedo Filipe aka Nox P@sNox <pasnox@gmail.com>, The Monkey Studio Team
** Project   : Monkey Studio 2
** FileName  : QDesignerWidgetBox.cpp
** Date      : 2007-11-04T22:53:19
** License   : GPL
** Comment   : Monkey Studio is a Free, Fast and lightweight crossplatform Qt RAD.
It's extendable with a powerfull plugins system.
** Home Page : http://www.monkeystudio.org
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/#include "QDesignerWidgetBox.h"
#include "QtDesignerChild.h"

#include <QDir>
#include <QDesignerComponents>
#include <QDesignerWidgetBoxInterface>
#include <QDesignerFormEditorInterface>

QDesignerWidgetBox::QDesignerWidgetBox( QtDesignerChild* p )
 : QDockWidget( 0 )
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
