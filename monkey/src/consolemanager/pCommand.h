/****************************************************************************
**
** 		Created using Monkey Studio v1.8.1.0
** Authors   : Filipe AZEVEDO aka Nox P@sNox <pasnox@gmail.com>
** Project   : Monkey Studio IDE
** FileName  : pCommand.h
** Date      : 2008-01-14T00:36:49
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
/*!
	\file pCommand.h
	\date 2008-01-14T00:36:50
	\author Filipe AZEVEDO aka Nox PasNox <pasnox@gmail.com>
	\brief Header for pCommand
*/
#ifndef PCOMMAND_H
#define PCOMMAND_H

#include <fresh.h>

#include <QStringList>

class XUPItem;

/*!
	Container for storing console command
	
	pCommand can store command line for executing command, working dirrectory,
	options of execution of command, can define parsers, which could be used 
	for executing, and also can remember project, for which command is 
	executing
*/
class Q_MONKEY_EXPORT pCommand
{
public:
	pCommand() 
	{
		mSkipOnError = false;
		mTryAllParsers = false;
		mProject = 0;
	}
	
	pCommand( const QString& t, const QString& c, const QString& a, bool b = false, const QStringList& p = QStringList(), const QString& d = QString::null, bool bb = false )
	{
		mText = t;
		mCommand = c;
		mArguments = a;
		mSkipOnError = b;
		mParsers = p;
		mWorkingDirectory = d;
		mTryAllParsers = bb;
		mProject = 0;
	}
	~pCommand() {}
	
	bool isValid() const
	{ return !text().isEmpty() && !command().isEmpty(); }
	
	bool operator==( const pCommand& t ) const
	{ return text() == t.text() && command() == t.command() && arguments() == t.arguments() &&
			workingDirectory() == t.workingDirectory() && parsers() == t.parsers() && skipOnError() == t.skipOnError() &&
			tryAllParsers() == t.tryAllParsers() && userData() == t.userData() && project() == t.project(); }

	QString text() const { return mText; }
	QString command() const { return mCommand; }
	QString arguments() const { return mArguments; }
	QString workingDirectory() const { return mWorkingDirectory; }
	QStringList parsers() const { return mParsers; }
	bool skipOnError() const { return mSkipOnError; }
	bool tryAllParsers() const { return mTryAllParsers; }
	QVariant userData() const { return mUserData; }
	XUPItem* project() const { return mProject; }

	void setText( const QString& s ) { mText = s; }
	void setCommand( const QString& s ) { mCommand = s; }
	void setArguments( const QString& s ) { mArguments = s; }
	void setWorkingDirectory( const QString& s ) { mWorkingDirectory = s; }
	void addParser( const QString& p ) { if ( !mParsers.contains( p ) ) mParsers << p; }
	void setParsers( const QStringList& p ) { mParsers = p; }
	void addParsers( const QStringList& p ) { foreach ( QString s, p ) addParser( s ); }
	void setSkipOnError( bool b ) { mSkipOnError = b; }
	void setTryAllParsers( bool b ) { mTryAllParsers = b; }
	void setUserData( const QVariant& data ) { mUserData = data; }
	void setProject( XUPItem* project ) { mProject = project; }
	
	QString toString() const
	{
		QString s;
		s += QString( "mText: %1\n" ).arg( mText );
		s += QString( "mCommand: %1\n" ).arg( mCommand );
		s += QString( "mArguments: %1\n" ).arg( mArguments );
		s += QString( "mWorkingDirectory: %1\n" ).arg( mWorkingDirectory );
		s += QString( "mSkipOnError: %1\n" ).arg( mSkipOnError );
		s += QString( "mParsers: %1\n" ).arg( mParsers.join( " " ) );
		s += QString( "mTryAllParsers: %1\n" ).arg( mTryAllParsers );
		s += QString( "mUserData: %1\n" ).arg( mUserData.toString() );
		s += QString( "mProject: %1" ).arg( (quintptr)mProject );
		return s;
	}
	
	void debug()
	{ qWarning( toString().toLocal8Bit().constData() ); }

protected:
	QString mText; 				/**< Comment about command */
	QString mCommand;			/**< Console command */
	QString mArguments;			/**< Arguments */
	QString mWorkingDirectory;	/**< Working dirrectory of process */
	bool mSkipOnError;			/**< Skip command, if error ocurred */
	QStringList mParsers;		/**< List of parsers, which should be used for command. Position in the list is not ignored */
	bool mTryAllParsers;		/**< Try to use all availible parsers after parsers from list */
	QVariant mUserData;			/**< Ask PasNox, hot to use it */
	XUPItem* mProject;			/**< Project, for which command is executing */
};

/*!
	List of pCommand objects
*/
typedef QList<pCommand> pCommandList;

Q_DECLARE_METATYPE( pCommand );
Q_DECLARE_METATYPE( pCommandList );

#endif // PCOMMAND_H
