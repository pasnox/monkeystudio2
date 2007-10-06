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

#include <QObject>
#include <QStringList>

class Q_MONKEY_EXPORT pCommand : public QObject
{
	Q_OBJECT

public:
	pCommand( QObject* o = 0 )	: QObject( o ) {}
	~pCommand() {}

	QString text() const { return mText; }
	QString command() const { return mCommand; }
	QStringList arguments() const { return mArguments; }
	QString workingDirectory() const { return mWorkingDirectory; }
	QStringList parsers() const { return mParsers; }
	bool skipOnError() const { return mSkipOnError; }
	bool tryAllParsers() const { return mTryAllParsers; }
	// add by hlamer
	QString tool() const { return mTool; }
	QString defaultCommand() const { return mTool; }

	void setText( const QString& s ) { mText = s; }
	void setCommand( const QString& s ) { mCommand = s; }
	void setArguments( const QStringList& l ) { mArguments = l; }
	void setWorkingDirectory( const QString& s ) { mWorkingDirectory = s; }
	void setParsers( const QStringList& p ) { mParsers = p; }
	void setSkipOnError( bool b ) { mSkipOnError = b; }
	void setTryAllParsers( bool b ) { mTryAllParsers = b; }
	// add by hlamer
	void setTool( const QString& s ) { mTool = s; }
	void setDefaultCommand( const QString& s ) { mTool = s; }

protected:
	QString mText;
	QString mCommand;
	QStringList mArguments;
	QString mWorkingDirectory;
	bool mSkipOnError;
	QStringList mParsers;
	bool mTryAllParsers;

	// add by hlamer
	QString mTool;
	QString mDefaultCommand;

};

typedef QList<pCommand*> pCommandList;

#endif // PCOMMAND_H
