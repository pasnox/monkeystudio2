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

#include <QObject>
#include <QStringList>
#include <QPointer>
#include <pCommandParser.h>

class pCommand : public QObject
{
	Q_OBJECT

public:
	pCommand( QObject* = 0 );
	~pCommand();

	QString text() const { return mText; }
	QString command() const { return mCommand; }
	QStringList arguments() const { return mArguments; }
	QString workingDirectory() const { return mWorkingDirectory; }
	pCommandParser* parser() const { return mParser; }
	bool skipOnError() const { return mSkipOnError; }

	void setText( const QString& s ) { mText = s; }
	void setCommand( const QString& s ) { mCommand = s; }
	void setArguments( const QStringList& l ) { mArguments = l; }
	void setWorkingDirectory( const QString& s ) { mWorkingDirectory = s; }
	void setParser( pCommandParser* p ) { mParser = p; }
	void setSkipOnError( bool b ) { mSkipOnError = b; }

protected:
	QString mText;
	QString mCommand;
	QStringList mArguments;
	QString mWorkingDirectory;
	bool mSkipOnError;
	QPointer<pCommandParser> mParser;

};

typedef QList<pCommand*> QCommandList;

#endif // PCOMMAND_H
