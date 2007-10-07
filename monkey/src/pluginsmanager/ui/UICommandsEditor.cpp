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
#include  "pConsoleManager.h"

UICommandsEditor::UICommandsEditor( const pCommandList& d, const pCommandList& c, QWidget* w )
	: QWidget( w )
{
	setupUi( this );
	// delete widget when close
	setAttribute( Qt::WA_DeleteOnClose );
	// memorize defaults and user commands
	mDefaults = d;
	mCommands = c;
	if ( mCommands.isEmpty() )
		mCommands = mDefaults;
	// load commands
	loadCommands();
	// add parsers
	lwCommandParsers->addItems( pConsoleManager::instance()->parsersName() );
	// set uncheck state
	for ( int i = 0; i < lwCommandParsers->count(); i++ )
		lwCommandParsers->item( i )->setCheckState( Qt::Unchecked );
	// select first item if available
	if ( lwCommands->count() )
		lwCommands->setCurrentRow( 0 );
}

UICommandsEditor::~UICommandsEditor()
{}

void UICommandsEditor::loadCommands()
{
	foreach ( pCommand c, mCommands )
		lwCommands->addItem( c.text() );
}

void UICommandsEditor::on_lwCommands_itemSelectionChanged()
{
	QListWidgetItem* it = lwCommands->selectedItems().value( 0 );
	pCommand c = it ? mCommands.value( lwCommands->row( it ) ) : pCommand();
	leCommandText->setText( c.text() );
	leCommandCommand->setText( c.command() );
	leCommandArguments->setText( c.arguments() );
	leCommandWorkingDirectory->setText( c.workingDirectory() );
	for ( int i = 0; i < lwCommandParsers->count(); i++ )
	{
		it = lwCommandParsers->item( i );
		it->setCheckState( c.parsers().contains( it->text() ) ? Qt::Checked : Qt::Unchecked );
	}
}
