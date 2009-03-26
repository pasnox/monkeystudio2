/****************************************************************************
**
** 		Created using Monkey Studio v1.8.1.0
** Authors    : Filipe AZEVEDO aka Nox P@sNox <pasnox@gmail.com>
** Project   : Monkey Studio IDE
** FileName  : pFileManager.cpp
** Date      : 2008-01-14T00:37:20
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
#include "pFileManager.h"
#include "pWorkspace.h"
#include "../xupmanager/gui/XUPProjectManager.h"
#include "../xupmanager/core/XUPProjectItem.h"
#include "pAbstractChild.h"
#include "../coremanager/MonkeyCore.h"
#include "pMonkeyStudio.h"
#include "../settingsmanager/Settings.h"
#include "../queuedstatusbar/QueuedStatusBar.h"
#include "../shellmanager/MkSShellInterpreter.h"

pFileManager::pFileManager( QObject* o )
	: QObject( o )
{
	initialize();
	
	// files
	connect( MonkeyCore::workspace(), SIGNAL( fileOpened( const QString& ) ), this, SIGNAL( fileOpened( const QString& ) ) );
	connect( MonkeyCore::workspace(), SIGNAL( fileClosed( const QString& ) ), this, SIGNAL( fileClosed( const QString& ) ) );
	connect( MonkeyCore::workspace(), SIGNAL( currentFileChanged( pAbstractChild*, const QString& ) ), this, SIGNAL( currentFileChanged( pAbstractChild*, const QString& ) ) );
	// projects
	connect( MonkeyCore::projectsManager(), SIGNAL( projectOpened( XUPProjectItem* ) ), this, SIGNAL( opened( XUPProjectItem* ) ) );
	connect( MonkeyCore::projectsManager(), SIGNAL( projectAboutToClose( XUPProjectItem* ) ), this, SIGNAL( aboutToClose( XUPProjectItem* ) ) );
	connect( MonkeyCore::projectsManager(), SIGNAL( currentProjectChanged( XUPProjectItem* ) ), this, SIGNAL( currentChanged( XUPProjectItem* ) ) );
	connect( MonkeyCore::projectsManager(), SIGNAL( currentProjectChanged( XUPProjectItem*, XUPProjectItem* ) ), this, SIGNAL( currentChanged( XUPProjectItem*, XUPProjectItem* ) ) );
}

void pFileManager::initialize()
{
	// register command
	QString help = MkSShellInterpreter::tr
	(
		"This command manage the associations, usage:\n"
		"The suffixes are a comma separated list of suffixes (ie: \"*.txt, *.doc\")\n"
		"\tassociation add [type] [suffixes]\n"
		"\tassociation set [type] [suffixes]\n"
		"\tassociation del [type] [suffixes]\n"
		"\tassociation list [type] -- If type is missing, all suffixes will be shown.\n"
		"\tassociation clear [type] -- If type is missing, all suffixes will be cleared."
	);
	
	MonkeyCore::interpreter()->addCommandImplementation( "association", pFileManager::commandInterpreter, help );
}

QString pFileManager::commandInterpreter( const QString& command, const QStringList& arguments, int* result, MkSShellInterpreter* interpreter )
{
	Q_UNUSED( command );
	Q_UNUSED( interpreter );
	const QStringList allowedOperations = QStringList( "add" ) << "set" << "del" << "list" << "clear";
	
	if ( result )
	{
		*result = 0;
	}
	
	if ( arguments.isEmpty() )
	{
		if ( result )
		{
			*result = MkSShellInterpreter::InvalidCommand;
		}
		
		return MkSShellInterpreter::tr( "Operation not defined. Available operations are: %1." ).arg( allowedOperations.join( ", " ) );
	}
	
	const QString operation = arguments.first();
	
	if ( !allowedOperations.contains( operation ) )
	{
		if ( result )
		{
			*result = MkSShellInterpreter::InvalidCommand;
		}
		
		return MkSShellInterpreter::tr( "Unknown operation: '%1'." ).arg( operation );
	}
	
	if ( operation == "add" )
	{
		if ( arguments.count() != 3 )
		{
			if ( result )
			{
				*result = MkSShellInterpreter::InvalidCommand;
			}
			
			return MkSShellInterpreter::tr( "'add' operation take 2 arguments, %1 given." ).arg( arguments.count() -1 );
		}
		
		const QString type = arguments.at( 1 );
		const QStringList suffixes = arguments.at( 2 ).split( ",", QString::SkipEmptyParts );
		
		MonkeyCore::fileManager()->add( type, suffixes );
	}
	
	if ( operation == "set" )
	{
		if ( arguments.count() != 3 )
		{
			if ( result )
			{
				*result = MkSShellInterpreter::InvalidCommand;
			}
			
			return MkSShellInterpreter::tr( "'set' operation take 2 arguments, %1 given." ).arg( arguments.count() -1 );
		}
		
		const QString type = arguments.at( 1 );
		const QStringList suffixes = arguments.at( 2 ).split( ",", QString::SkipEmptyParts );
		
		MonkeyCore::fileManager()->set( type, suffixes );
	}
	
	if ( operation == "del" )
	{
		if ( arguments.count() != 3 )
		{
			if ( result )
			{
				*result = MkSShellInterpreter::InvalidCommand;
			}
			
			return MkSShellInterpreter::tr( "'del' operation take 2 arguments, %1 given." ).arg( arguments.count() -1 );
		}
		
		const QString type = arguments.at( 1 );
		const QStringList suffixes = arguments.at( 2 ).split( ",", QString::SkipEmptyParts );
		
		MonkeyCore::fileManager()->remove( type, suffixes );
	}
	
	if ( operation == "list" )
	{
		if ( arguments.count() > 2 )
		{
			if ( result )
			{
				*result = MkSShellInterpreter::InvalidCommand;
			}
			
			return MkSShellInterpreter::tr( "'list' operation take 0 or 1 argument, %1 given." ).arg( arguments.count() -1 );
		}
		
		const QString type = arguments.value( 1 );
		QStringList output;
		
		if ( type.isNull() )
		{
			foreach ( const QString& ctype, MonkeyCore::fileManager()->associations().keys() )
			{
				output << QString( "%1:" ).arg( ctype );
				output << QString( "\t%1" ).arg( MonkeyCore::fileManager()->associations( ctype ).join( ", " ) );
			}
		}
		else
		{
			output << QString( "%1:" ).arg( type );
			output << QString( "\t%1" ).arg( MonkeyCore::fileManager()->associations( type ).join( ", " ) );
		}
		
		if ( !output.isEmpty() )
		{
			output.prepend( MkSShellInterpreter::tr( "Found associations:" ) );
		}
		else
		{
			output << MkSShellInterpreter::tr( "No associations found." );
		}
		
		return output.join( "\n" );
	}
	
	if ( operation == "clear" )
	{
		if ( arguments.size() > 2 )
		{
			if ( result )
			{
				*result = MkSShellInterpreter::InvalidCommand;
			}
			
			return MkSShellInterpreter::tr( "'clear' operation take 0 or 1 argument, %1 given." ).arg( arguments.count() -1 );
		}
		
		const QString type = arguments.value( 1 );
		
		MonkeyCore::fileManager()->clear( type );
	}
	
	return QString::null;
}

void pFileManager::clear( const QString& type )
{
	if ( type.isNull() )
	{
		mAssociations.clear();
	}
	else
	{
		mAssociations.remove( type );
	}
}

void pFileManager::add( const QString& type, const QStringList& suffixes )
{
	foreach ( const QString& suffix, suffixes )
	{
		if ( !mAssociations[ type ].contains( suffix ) )
		{
			mAssociations[ type ] << suffix;
		}
	}
}

void pFileManager::add( const QString& type, const QString& suffix )
{
	add( type, QStringList( suffix ) );
}

void pFileManager::set( const QString& type, const QStringList& suffixes )
{
	clear( type );
	add( type, suffixes );
}

void pFileManager::set( const QString& type, const QString& suffix )
{
	set( type, QStringList( suffix ) );
}

void pFileManager::remove( const QString& type, const QStringList& suffixes )
{
	QStringList result = associations( type );
	
	foreach ( const QString& suffix, suffixes )
	{
		result.removeOne( suffix );
	}
	
	if ( result.isEmpty() )
	{
		clear( type );
	}
	else
	{
		mAssociations[ type ] = result;
	}
}

void pFileManager::remove( const QString& type, const QString& suffix )
{
	remove( type, QStringList( suffix ) );
}

const QMap<QString, QStringList>& pFileManager::associations() const
{
	return mAssociations;
}

QStringList pFileManager::associations( const QString& type ) const
{
	return mAssociations.value( type );
}

void pFileManager::generateScript()
{
	// write content in utf8
	const QString fn = MonkeyCore::settings()->homePath( Settings::SP_SCRIPTS ).append( "/associations.mks" );
	QFile file( fn );
	QStringList buffer;
	
	if ( !file.open( QIODevice::WriteOnly ) )
	{
		MonkeyCore::statusBar()->appendMessage( tr( "Can't open file for generating associations script: %1" ).arg( file.errorString() ) );
		return;
	}
	
	file.resize( 0 );
	
	buffer << "# Monkey Studio IDE Associations";
	buffer << "# reset associations";
	buffer << "association clear";
	buffer << "# introduce new ones per language";
	buffer << "# association add\tlanguage/type\tSuffixes";
	buffer << "# association set\tlanguage/type\tSuffixes";
	
	foreach ( const QString& type, mAssociations.keys() )
	{
		buffer << QString( "# %1" ).arg( type );
		
		if ( !mAssociations[ type ].isEmpty() )
		{
			buffer << QString( "association set \"%1\" \"%2\"" )
				.arg( type )
				.arg( mAssociations[ type ].join( ", " ) );
		}
	}
	
	if ( file.write( buffer.join( "\n" ).toUtf8() ) == -1 )
	{
		MonkeyCore::statusBar()->appendMessage( tr( "Can't write generated associations script: %1" ).arg( file.errorString() ) );
	}
	
	file.close();
}

pAbstractChild* pFileManager::childForFile( const QString& file ) const
{
	foreach ( pAbstractChild* ac, MonkeyCore::workspace()->children() )
	{
		foreach ( const QString& fn, ac->files() )
		{
			if ( pMonkeyStudio::isSameFile( fn, file ) )
			{
				return ac;
			}
		}
	}
	
	return 0;
}

QString pFileManager::fileBuffer( const QString& fileName, const QString& codec ) const
{
	QString result;
	bool ok;
	
	foreach ( pAbstractChild* ac, MonkeyCore::workspace()->children() )
	{
		result = ac->fileBuffer( fileName, ok );
		
		if ( ok )
		{
			return result;
		}
	}
	
	result.clear();
	QFile file( fileName );
	
	if ( file.exists() )
	{
		if ( file.open( QIODevice::ReadOnly ) )
		{
			QTextCodec* c = QTextCodec::codecForName( codec.toUtf8() );
			result = c->toUnicode( file.readAll() );
			file.close();
		}
	}
	
	return result;
}

XUPProjectItem* pFileManager::currentProject() const
{ return MonkeyCore::projectsManager()->currentProject(); }

QString pFileManager::currentProjectFile() const
{ return currentProject() ? currentProject()->fileName() : QString(); }

QString pFileManager::currentProjectPath() const
{ return currentProject() ? currentProject()->path() : QString(); }

pAbstractChild* pFileManager::currentChild() const
{ return MonkeyCore::workspace()->currentChild(); }

QString pFileManager::currentChildFile() const
{ return currentChild() ? currentChild()->currentFile() : QString(); }

QString pFileManager::currentChildPath() const
{ return QFileInfo( currentChildFile() ).path(); }

XUPItem* pFileManager::currentItem() const
{ return MonkeyCore::projectsManager()->currentItem(); }

QString pFileManager::currentItemFile() const
{
	XUPItem* item = currentItem();
	
	if ( item && item->type() == XUPItem::File )
	{
		QString fn = item->project()->rootIncludeProject()->interpretValue( item, "content" );
		return item->project()->filePath( fn );
	}
	
	return QString::null;
}

QString pFileManager::currentItemPath() const
{
	XUPItem* item = currentItem();
	
	if ( item && item->type() == XUPItem::Path )
	{
		QString fn = item->project()->rootIncludeProject()->interpretValue( item, "content" );
		return item->project()->filePath( fn );
	}
	
	return QString::null;
}

pAbstractChild* pFileManager::openFile( const QString& fileName, const QString& codec )
{ return MonkeyCore::workspace()->openFile( fileName, codec ); }

void pFileManager::closeFile( const QString& s )
{ MonkeyCore::workspace()->closeFile( s ); }

void pFileManager::goToLine( const QString& s, const QPoint& p, bool b, const QString& codec )
{ MonkeyCore::workspace()->goToLine( s, p, b, codec ); }

void pFileManager::openProject( const QString& fileName, const QString& codec )
{
	MonkeyCore::projectsManager()->openProject( fileName, codec );
}
