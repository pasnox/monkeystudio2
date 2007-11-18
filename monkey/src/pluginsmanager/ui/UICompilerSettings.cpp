/****************************************************************************
**
** 		Created using Monkey Studio v1.7.0
** Author    : Azevedo Filipe aka Nox P@sNox <pasnox@gmail.com>
** Project   : UICompilerSettings
** FileName  : UICompilerSettings.cpp
** Date      : 2007-10-07T02:09:14
** License   : GPL
** Comment   : Your comment here
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/
#include "UICompilerSettings.h"
#include "CompilerPlugin.h"
#include  "pConsoleManager.h"
#include "pMonkeyStudio.h"

#include <QInputDialog>
#include <QPushButton>

using namespace pMonkeyStudio;

UICompilerSettings::UICompilerSettings( CompilerPlugin* p, QWidget* w )
	: QWidget( w ), mPlugin( p )
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
	mDefault = mPlugin->defaultCompileCommand();
	mCommand = mPlugin->compileCommand();
	mReset = mCommand;
	// add parsers
	lwBuildCommandParsers->addItems( pConsoleManager::instance()->parsersName() );
	// set uncheck state for parser items
	for ( int i = 0; i < lwBuildCommandParsers->count(); i++ )
		lwBuildCommandParsers->item( i )->setCheckState( Qt::Unchecked );
	// load commands
	updateCommand();
}

void UICompilerSettings::updateCommand()
{
	leBuildCommandText->setText( mCommand.text() );
	leBuildCommandCommand->setText( mCommand.command() );
	leBuildCommandArguments->setText( mCommand.arguments() );
	leBuildCommandWorkingDirectory->setText( mCommand.workingDirectory() );
	cbBuildCommandSkipOnError->setChecked( mCommand.skipOnError() );
	for ( int i = 0; i < lwBuildCommandParsers->count(); i++ )
	{
		QListWidgetItem* it = lwBuildCommandParsers->item( i );
		it->setCheckState( mCommand.parsers().contains( it->text() ) ? Qt::Checked : Qt::Unchecked );
	}
	cbBuildCommandTryAll->setChecked( mCommand.tryAllParsers() );
}

void UICompilerSettings::restoreDefault()
{
	mCommand = mDefault;
	updateCommand();
}

void UICompilerSettings::reset()
{
	mCommand = mReset;
	updateCommand();
}

void UICompilerSettings::save()
{
	mCommand.setText( leBuildCommandText->text() );
	mCommand.setCommand( leBuildCommandCommand->text() );
	mCommand.setArguments( leBuildCommandArguments->text() );
	mCommand.setWorkingDirectory( leBuildCommandWorkingDirectory->text() );
	mCommand.setSkipOnError( cbBuildCommandSkipOnError->isChecked() );
	QStringList l;
	for ( int i = 0; i < lwBuildCommandParsers->count(); i++ )
	{
		QListWidgetItem* it = lwBuildCommandParsers->item( i );
		if ( it->checkState() == Qt::Checked )
			l << it->text();
	}
	mCommand.setParsers( l );
	mCommand.setTryAllParsers( cbBuildCommandTryAll->isChecked() );
	mPlugin->setCompileCommand( mCommand );
}

void UICompilerSettings::on_tbBuildCommandCommand_clicked()
{
	QString s = getOpenFileName( tr( "Select an executable" ), leBuildCommandCommand->text() );
	if ( !s.isNull() )
		leBuildCommandCommand->setText( s );
}

void UICompilerSettings::on_tbBuildCommandWorkingDirectory_clicked()
{
	QString s = getExistingDirectory( tr( "Select a folder" ), leBuildCommandWorkingDirectory->text() );
	if ( !s.isNull() )
		leBuildCommandWorkingDirectory->setText( s );
}

void UICompilerSettings::on_dbbButtons_clicked( QAbstractButton* b )
{
	if ( dbbButtons->standardButton( b ) == QDialogButtonBox::Reset )
		reset();
	else if ( dbbButtons->standardButton( b ) == QDialogButtonBox::RestoreDefaults )
		restoreDefault();
	else if ( dbbButtons->standardButton( b ) == QDialogButtonBox::Save )
		save();
}
