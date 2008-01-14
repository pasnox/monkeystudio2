/****************************************************************************
**
** 		Created using Monkey Studio v1.8.1.0
** Authors    : Filipe AZEVEDO aka Nox P@sNox <pasnox@gmail.com>
** Project   : Monkey Studio Child Plugins
** FileName  : QDesignerObjectInspector.cpp
** Date      : 2008-01-14T00:57:15
** License   : GPL
** Comment   : This header has been automatically generated, if you are the original author, or co-author, fill free to replace/append with your informations.
** Home Page : http://www.monkeystudio.org
**
    Copyright (C) 2005 - 2008  Filipe AZEVEDO & The Monkey Studio Team

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
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
