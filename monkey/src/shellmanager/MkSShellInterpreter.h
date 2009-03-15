#ifndef MKSSHELLINTERPRETER_H
#define MKSSHELLINTERPRETER_H

#include <MonkeyExport.h>
#include "pConsoleCommand.h"

#include <QPointer>
#include <QObject>
#include <QHash>

/*
Pointer to function
QString commandImplementation( const QString& command, const QStringList& arguments, int* result, MkSShellInterpreter* interpreter )
*/
typedef QString (*CommandImplementationPtr)(const QString&, const QStringList&, int*, class MkSShellInterpreter* );

class Q_MONKEY_EXPORT MkSShellInterpreter : public QObject, public pConsoleCommand
{
	Q_OBJECT
	
public:
	enum Error
	{
		InvalidCommand = -1,
		NoResultVariable = -2
	};
	
	static MkSShellInterpreter* instance( QObject* parent = 0 );
	static QString sourceScriptsPath();
	static QString targetScriptsPath();
	
	QString usage( const QString& command ) const;
	QString interpret( const QString& command, int* result ) const;
	
	void addCommandImplementation( const QString& command, CommandImplementationPtr function, const QString& help = QString::null );
	void setCommandHelp( const QString& command, const QString& help );
	
protected:
	static QPointer<MkSShellInterpreter> mInstance;
	QHash<QString, CommandImplementationPtr> mCommandImplementations;
	QHash<QString, QString> mCommandHelps;
	
	MkSShellInterpreter( QObject* parent = 0 );
	static QString interpretHelp( const QString&, const QStringList& arguments, int* result, MkSShellInterpreter* interpreter );
	
signals:
	void commandExecuted( const QString& command, const QString& output, int result );
};

#endif // MKSSHELLINTERPRETER_H
