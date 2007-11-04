/****************************************************************************
**
** 		Created using Monkey Studio v1.8.0.0
** Author    : Azevedo Filipe aka Nox P@sNox <pasnox@gmail.com>, The Monkey Studio Team
** Project   : Monkey Studio 2
** FileName  : UICLIToolSettings.cpp
** Date      : 2007-11-04T22:45:23
** License   : GPL
** Comment   : Monkey Studio is a Free, Fast and lightweight crossplatform Qt RAD.
It's extendable with a powerfull plugins system.
** Home Page : http://www.monkeystudio.org
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/
#include "UICLIToolSettings.h"
#include "CompilerPlugin.h"
#include  "pConsoleManager.h"
#include "pMonkeyStudio.h"
#include "CLIToolPlugin.h"

#include <QInputDialog>

using namespace pMonkeyStudio;

UICLIToolSettings::UICLIToolSettings( BasePlugin* p, const pCommandList& d, const pCommandList& c, QWidget* w )
	: QWidget( w ), mPlugin( p ), mUpdating( false )
{
	Q_ASSERT( mPlugin );
	setupUi( this );
	// set button icons
	dbbButtons->button( QDialogButtonBox::Help )->setIcon( QIcon( ":/help/icons/help/keyword.png" ) );
	dbbButtons->button( QDialogButtonBox::Reset )->setIcon( QIcon( ":/tools/icons/tools/update.png" ) );
	dbbButtons->button( QDialogButtonBox::RestoreDefaults )->setIcon( QIcon( ":/file/icons/file/backup.png" ) );
	dbbButtons->button( QDialogButtonBox::Save )->setIcon( QIcon( ":/file/icons/file/save.png" ) );
	// delete widget when close
	setAttribute( Qt::WA_DeleteOnClose );
	// memorize defaults and user commands
	mDefaults = d;
	mCommands = c;
	if ( mCommands.isEmpty() )
		mCommands = mDefaults;
	mReset = mCommands;
	// add parsers
	lwCommandParsers->addItems( pConsoleManager::instance()->parsersName() );
	// set uncheck state for parser items
	for ( int i = 0; i < lwCommandParsers->count(); i++ )
		lwCommandParsers->item( i )->setCheckState( Qt::Unchecked );
	// load commands
	updateCommands();
}

void UICLIToolSettings::updateCommands()
{
	mUpdating = true;
	lwCommands->clear();
	foreach ( pCommand c, mCommands )
		lwCommands->addItem( c.text() );
	mUpdating = false;
	if ( lwCommands->count() )
		lwCommands->setCurrentRow( 0 );
}

void UICLIToolSettings::restoreDefaults()
{
	mCommands = mDefaults;
	updateCommands();
}

void UICLIToolSettings::reset()
{
	mCommands = mReset;
	updateCommands();
}

void UICLIToolSettings::save()
{
	on_lwCommands_currentItemChanged( lwCommands->currentItem(), lwCommands->currentItem() );
	dynamic_cast<CLIToolPlugin*>( mPlugin )->setUserCommands( mCommands );
}

void UICLIToolSettings::on_lwCommands_itemSelectionChanged()
{ lwCommands->setCurrentItem( lwCommands->selectedItems().value( 0 ) ); }

void UICLIToolSettings::on_lwCommands_currentItemChanged( QListWidgetItem* cit, QListWidgetItem* pit )
{
	if ( pit && !mUpdating )
	{
		pCommand& c = mCommands[ lwCommands->row( pit ) ];
		c.setText( leCommandText->text() );
		c.setCommand( leCommandCommand->text() );
		c.setArguments( leCommandArguments->text() );
		c.setWorkingDirectory( leCommandWorkingDirectory->text() );
		c.setSkipOnError( cbCommandSkipOnError->isChecked() );
		QStringList l;
		for ( int i = 0; i < lwCommandParsers->count(); i++ )
		{
			QListWidgetItem* it = lwCommandParsers->item( i );
			if ( it->checkState() == Qt::Checked )
				l << it->text();
		}
		c.setParsers( l );
		c.setTryAllParsers( cbCommandTryAll->isChecked() );
		pit->setText( c.text() );
	}
	pCommand c = cit ? mCommands.value( lwCommands->row( cit ) ) : pCommand();
	leCommandText->setText( c.text() );
	leCommandCommand->setText( c.command() );
	leCommandArguments->setText( c.arguments() );
	leCommandWorkingDirectory->setText( c.workingDirectory() );
	cbCommandSkipOnError->setChecked( c.skipOnError() );
	for ( int i = 0; i < lwCommandParsers->count(); i++ )
	{
		cit = lwCommandParsers->item( i );
		cit->setCheckState( c.parsers().contains( cit->text() ) ? Qt::Checked : Qt::Unchecked );
	}
	cbCommandTryAll->setChecked( c.tryAllParsers() );
}

void UICLIToolSettings::on_pbCommandAdd_clicked()
{
	pCommand c( tr( "New Command" ), "command", "arguments" );
	mCommands << c;
	lwCommands->addItem( c.text() );
	lwCommands->setCurrentRow( lwCommands->count() -1 );
}

void UICLIToolSettings::on_pbCommandRemove_clicked()
{
	if ( QListWidgetItem* it = lwCommands->currentItem() )
	{
		mUpdating = true;
		int i = lwCommands->row( it );
		delete it;
		mCommands.removeAt( i );
		mUpdating = false;
	}
}

void UICLIToolSettings::on_pbCommandUp_clicked()
{
	if ( QListWidgetItem* it = lwCommands->currentItem() )
	{
		int i = lwCommands->row( it );
		if ( i > 0 )
		{
			lwCommands->insertItem( i -1, lwCommands->takeItem( i ) );
			mCommands.swap( i, i -1 );
			lwCommands->setCurrentItem( it );
		}
	}
}

void UICLIToolSettings::on_pbCommandDown_clicked()
{
	if ( QListWidgetItem* it = lwCommands->currentItem() )
	{
		int i = lwCommands->row( it );
		if ( i < lwCommands->count() -1 )
		{
			lwCommands->insertItem( i +1, lwCommands->takeItem( i ) );
			mCommands.swap( i, i +1 );
			lwCommands->setCurrentItem( it );
		}
	}
}

void UICLIToolSettings::on_tbCommandCommand_clicked()
{
	QString s = getOpenFileName( tr( "Select an executable" ), leCommandCommand->text() );
	if ( !s.isNull() )
		leCommandCommand->setText( s );
}

void UICLIToolSettings::on_tbCommandWorkingDirectory_clicked()
{
	QString s = getExistingDirectory( tr( "Select a folder" ), leCommandWorkingDirectory->text() );
	if ( !s.isNull() )
		leCommandWorkingDirectory->setText( s );
}

void UICLIToolSettings::on_dbbButtons_clicked( QAbstractButton* b )
{
	if ( dbbButtons->standardButton( b ) == QDialogButtonBox::Reset )
		reset();
	else if ( dbbButtons->standardButton( b ) == QDialogButtonBox::RestoreDefaults )
		restoreDefaults();
	else if ( dbbButtons->standardButton( b ) == QDialogButtonBox::Save )
		save();
}
