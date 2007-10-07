/****************************************************************************
**
** 		Created using Monkey Studio v1.7.0
** Author    : Azevedo Filipe aka Nox P@sNox <pasnox@gmail.com>
** Project   : UICommandsEditor
** FileName  : UICommandsEditor.cpp
** Date      : 2007-10-07T02:09:14
** License   : GPL
** Comment   : Your comment here
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/
#include "UICommandsEditor.h"

UICommandsEditor::UICommandsEditor( const pCommandList& d, const pCommandList& c, QWidget* w )
	: QWidget( w )
{
	// delete widget when close
	setAttribute( Qt::WA_DeleteOnClose );
	// memorize defaults and user commands
	mDefaults = d;
	mCommands = c;
	if ( mCommands.isEmpty() )
		mCommands = mDefaults;
	// load commands
	loadCommands();
}

UICommandsEditor::~UICommandsEditor()
{}

void UICommandsEditor::loadCommands()
{
	foreach ( pCommand c, mCommands )
		lwCommands->addItem( c.text() );
}
