/****************************************************************************
**
** 		Created using Monkey Studio v1.8.1.0
** Authors    : Filipe AZEVEDO aka Nox P@sNox <pasnox@gmail.com>
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
#ifndef PCOMMAND_H
#define PCOMMAND_H

#include "MonkeyExport.h"

#include <QStringList>

class Q_MONKEY_EXPORT pCommand
{
public:
	pCommand() 
	{
		mSkipOnError = false;
		mTryAllParsers = false;
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
	}
	~pCommand() {}
	
	bool isValid() const
	{ return !text().isEmpty() && !command().isEmpty(); }
	
	bool operator==( const pCommand& t )
	{ return text() == t.text() && command() == t.command() && arguments() == t.arguments() &&
			workingDirectory() == t.workingDirectory() && parsers() == t.parsers() && skipOnError() == t.skipOnError() &&
			tryAllParsers() == t.tryAllParsers(); }

	QString text() const { return mText; }
	QString command() const { return mCommand; }
	QString arguments() const { return mArguments; }
	QString workingDirectory() const { return mWorkingDirectory; }
	QStringList parsers() const { return mParsers; }
	bool skipOnError() const { return mSkipOnError; }
	bool tryAllParsers() const { return mTryAllParsers; }

	void setText( const QString& s ) { mText = s; }
	void setCommand( const QString& s ) { mCommand = s; }
	void setArguments( const QString& s ) { mArguments = s; }
	void setWorkingDirectory( const QString& s ) { mWorkingDirectory = s; }
	void addParser( const QString& p ) { if ( !mParsers.contains( p ) ) mParsers << p; }
	void setParsers( const QStringList& p ) { mParsers = p; }
	void addParsers( const QStringList& p ) { foreach ( QString s, p ) addParser( s ); }
	void setSkipOnError( bool b ) { mSkipOnError = b; }
	void setTryAllParsers( bool b ) { mTryAllParsers = b; }

protected:
	QString mText;
	QString mCommand;
	QString mArguments;
	QString mWorkingDirectory;
	bool mSkipOnError;
	QStringList mParsers;
	bool mTryAllParsers;

};

typedef QList<pCommand> pCommandList;

#endif // PCOMMAND_H
