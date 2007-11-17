/****************************************************************************
**
** 		Created using Monkey Studio v1.7.0
** Author    : P@sNox
** Project   : pCommand
** FileName  : pCommand.h
** Date      : jeu. 23. août 15:20:31 2007
** License   : GPL
** Comment   : Your comment here
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
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
		mSkipOnError = true; //FIXME
		mTryAllParsers = true; //FIXME
	}
	
	pCommand( const QString& t, const QString& c, const QString& a, bool b = false, const QStringList& p = QStringList(), const QString& d = QString::null )
	{
		mText = t;
		mCommand = c;
		mArguments = a;
		mSkipOnError = b;
		mParsers = p;
		mWorkingDirectory = d;
		mTryAllParsers = true; //FIXME
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
	void setParsers( const QStringList& p ) { mParsers = p; }
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
