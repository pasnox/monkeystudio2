/****************************************************************************
**
** 		Created using Monkey Studio v1.8.0.0
** Author    : Azevedo Filipe aka Nox P@sNox <pasnox@gmail.com>
** Project   : ProjectHeaders
** FileName  : ProjectHeaders.cpp
** Date      : 2007-11-04T01:45:50
** License   : GPL
** Comment   : Your comment here
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/
#include "ProjectHeaders.h"
#include "MonkeyCore.h"
#include "pMenuBar.h"
#include "UIProjectHeaders.h"
#include "pMonkeyStudio.h"

using namespace pMonkeyStudio;

ProjectHeaders::ProjectHeaders()
{
	// set plugin infos
	mPluginInfos.Caption = tr( "Project Headers" );
	mPluginInfos.Description = tr( "Plugin for managing the license headers of your sources" );
	mPluginInfos.Author = "Azevedo Filipe aka Nox P@sNox <pasnox@gmail.com>";
	mPluginInfos.Type = BasePlugin::iBase;
	mPluginInfos.Name = "ProjectHeaders";
	mPluginInfos.Version = "0.5.0";
	mPluginInfos.Enabled = false;
}

ProjectHeaders::~ProjectHeaders()
{
	if ( isEnabled() )
		setEnabled( false );
}

bool ProjectHeaders::setEnabled( bool b )
{
	if ( b && !isEnabled() )
	{
		// add dock to dock toolbar entry
		QAction* a = MonkeyCore::menuBar()->action( "mEdit/aProjectHeaders", tr( "Project Licensing..." ), QIcon( ":/icons/licensing.png" ) );
		connect( a, SIGNAL( triggered() ), this, SLOT( processLicensing() ) );
		// set plugin enabled
		mPluginInfos.Enabled = true;
	}
	else if ( !b && isEnabled() )
	{
		// set plugin disabled
		mPluginInfos.Enabled = false;
	}
	
	// return default value
	return true;
}

void ProjectHeaders::setTemplatesHeaderRegExp( const QString& l, const QString& s )
{ setSettingsValue( QString( "RegExp/" ).append( l ), s ); }

QString ProjectHeaders::templatesHeaderRegExp( const QString& l ) const
{
	QString s = settingsValue( QString( "RegExp/" ).append( l ), QString() ).toString();
	return s.isEmpty() ? defaultTemplatesHeaderRegExp( l ) : s;
}

QString ProjectHeaders::defaultTemplatesHeaderRegExp( const QString& l ) const
{
	QString s;
	
	if ( l == "C++" )
		s = "^(/\\*[^$]*\\*/)";
	else if ( l == "HTML" )
		s = "^(<\\!--[^$]*-->)";
	
	// default
	return s;
}

void ProjectHeaders::setTemplatesHeader( const QString& l, const QString& s )
{ setSettingsValue( QString( "Licensing/" ).append( l ), s ); }

QString ProjectHeaders::templatesHeader( const QString& l ) const
{
	QString s = settingsValue( QString( "Licensing/" ).append( l ), QString() ).toString();
	return s.isEmpty() ? defaultTemplatesHeader( l ) : s;
}

QString ProjectHeaders::defaultTemplatesHeader( const QString& l ) const
{
	QString s;
	
	if ( l == "C++" )
		s = "/****************************************************************************\n"
			"**\n"
			"** 		Created using $editor_version_string$\n"
			"** Authors    : $authors$\n"
			"** Project   : $projectname$\n"
			"** FileName  : $filename$\n"
			"** Date      : $date$\n"
			"** License   : $license$\n"
			"** Comment   : $comment$\n"
			"** Home Page : $homepage$\n"
			"**\n"
			"** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE\n"
			"** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.\n"
			"**\n"
			"****************************************************************************/\n";
	else if ( l == "HTML" )
		s = "<!--\n"
			"/****************************************************************************\n"
			"**\n"
			"** 		Created using $editor_version_string$\n"
			"** Authors    : $authors$\n"
			"** Project   : $projectname$\n"
			"** FileName  : $filename$\n"
			"** Date      : $date$\n"
			"** License   : $license$\n"
			"** Comment   : $comment$\n"
			"** Home Page : $homepage$\n"
			"**\n"
			"** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE\n"
			"** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.\n"
			"**\n"
			"****************************************************************************/\n"
			"-->\n";
	
	// default
	return s;
}

void ProjectHeaders::processLicensing()
{
	// create dialog
	UIProjectHeaders d( 0, this );
	
	// add languages
	d.cbLanguages->addItems( availableLanguages() );
	
	// add regexp and licensing
	for ( int i = 0; i < d.cbLanguages->count(); i++ )
	{
		d.cbLanguages->setItemData( i, templatesHeaderRegExp( d.cbLanguages->itemText( i ) ), Qt::UserRole +1 );
		d.cbLanguages->setItemData( i, templatesHeader( d.cbLanguages->itemText( i ) ), Qt::UserRole +2 );
	}
	
	// show current language information
	if ( d.cbLanguages->count() )
	{
		d.cbLanguages->setCurrentIndex( 0 );
		d.leRegExp->setText( d.cbLanguages->itemData( 0, Qt::UserRole +1 ).toString() );
		d.teLicensing->setPlainText( d.cbLanguages->itemData( 0, Qt::UserRole +2 ).toString() );
	}
	
	// execute dialog
	d.exec();
	
	// save regexp and licensing
	for ( int i = 0; i < d.cbLanguages->count(); i++ )
	{
		setTemplatesHeaderRegExp( d.cbLanguages->itemText( i ), d.cbLanguages->itemData( i, Qt::UserRole +1 ).toString() );
		setTemplatesHeader( d.cbLanguages->itemText( i ), d.cbLanguages->itemData( i, Qt::UserRole +2 ).toString() );
	}
}

Q_EXPORT_PLUGIN2( BaseProjectHeaders, ProjectHeaders )
