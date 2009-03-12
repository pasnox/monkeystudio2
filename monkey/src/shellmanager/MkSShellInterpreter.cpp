#include "MkSShellInterpreter.h"
#include "GetOpt.h"

#include <QDebug>

QPointer<MkSShellInterpreter> MkSShellInterpreter::mInstance = 0;

QString interpretAbbreviation( const QString&, const QStringList& arguments, int* result, MkSShellInterpreter* interpreter )
{
	Q_UNUSED( interpreter );
	
	if ( arguments.isEmpty() )
	{
		*result = -1;
		return QString( "Operation not defined. Available operations is 'add', 'del', 'show'" );
	}
	
	if ( "add" != arguments[0] && 
		"del" != arguments[0] && 
		"show" != arguments[0] )
	{
		if ( result )
		{
			*result = MkSShellInterpreter::InvalidCommand;
		}
		
		return QString( "Unknown command: '%1'" ).arg( arguments[0] );
	}
	
	if ( arguments[0] == "add" )
	{
		if ( arguments.size() != 4 )
		{
			if ( result )
			{
				*result = MkSShellInterpreter::InvalidCommand;
			}
			
			return QString("'add' command has 4 arguments, %1 given").arg (arguments.size());
		}
		
		const QString macro = arguments [0];
		const QString description = arguments [1];
		const QString language = arguments [2];
		const QString code = arguments [3];
		
		if ( macro.isEmpty() || description.isEmpty() || language.isEmpty() || code.isEmpty())
		{
			if ( result )
			{
				*result = MkSShellInterpreter::InvalidCommand;
			}
			return  QString( "Can't add abbreviation '%1' for language '%2'." ).arg( macro ).arg( language );
		}
	}
	
	if (arguments[0] == "del")
	{
		//TODO
	}
	
	if (arguments[0] == "show")
	{
		//TODO
	}
	
	if ( result )
	{
		*result = 0;
	}
	
	return QString::null;
}

QString MkSShellInterpreter::interpretHelp( const QString& command, const QStringList& arguments, int* result, MkSShellInterpreter* interpreter )
{
	Q_UNUSED( command );
	
	if ( arguments.isEmpty() ) // all available commands
	{
		QStringList answer = QStringList( tr( "Available commands:" ) );
		answer << interpreter->mCommandImplementations.keys();
		
		if ( result )
		{
			*result = 0;
		}
		
		return answer.join( "\n" );
	}
	else if ( arguments.count() == 1 ) // help for command
	{
		const QString cmd = arguments.first();
		const QString& usage = interpreter->usage( cmd );
		
		if ( result )
		{
			*result = interpreter->mCommandHelps.contains( cmd ) ? 0 : InvalidCommand;
		}
		
		return usage;
	}
	
	// error
	if ( result )
	{
		*result = InvalidCommand;
	}
	
	return tr( "'help' command accepts only one parameter. %1 given" ).arg( arguments.count() );
}

MkSShellInterpreter* MkSShellInterpreter::instance( QObject* parent )
{
	if ( !mInstance )
	{
		mInstance = new MkSShellInterpreter( parent );
	}
	
	return mInstance;
}

MkSShellInterpreter::MkSShellInterpreter( QObject* parent )
	: QObject( parent ), pConsoleCommand()
{
	addCommandImplementation( "help", interpretHelp, tr( "Type 'help' and name of command" ) );
	addCommandImplementation( "abbreviation", interpretAbbreviation, tr( "Don't use\nthis funciton!!!" ) );
}

QString MkSShellInterpreter::usage( const QString& command ) const
{
	if ( mCommandHelps.contains( command ) )
	{
		return mCommandHelps[ command ];
	}
	
	return tr( "%1: No help available." ).arg( command );
}

QString MkSShellInterpreter::interpret( const QString& command, int* result ) const
{
	QStringList parts = parseCommand( command );
	
	if ( parts.isEmpty() || !mCommandImplementations.contains( parts.first() ) )
	{
		if ( result )
		{
			*result = InvalidCommand;
		}
		
		return tr( "Invalid command: %1" ).arg( command );
	}
	
	MkSShellInterpreter* instance = const_cast<MkSShellInterpreter*>( this );
	const QString cmd = parts.takeFirst();
	const QString commandOutput = mCommandImplementations[ cmd ]( cmd, parts, result, instance );
	
	emit instance->commandExecuted( command, commandOutput, result ? *result : NoResultVariable );
	
	return commandOutput;
}

void MkSShellInterpreter::addCommandImplementation( const QString& command, CommandImplementationPtr function, const QString& help )
{
	Q_ASSERT( !mCommands.contains( command ) );
	
	mCommands << command;
	mCommandImplementations[ command ] = function;
	
	setCommandHelp( command, help );
}
void MkSShellInterpreter::setCommandHelp( const QString& command, const QString& help )
{
	mCommandHelps[ command ] = help;
}
