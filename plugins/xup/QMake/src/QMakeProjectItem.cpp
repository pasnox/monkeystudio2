#include "QMakeProjectItem.h"
#include "QMake2XUP.h"
#include "../QtVersionManager.h"

#include <XUPProjectItemInfos.h>
#include <pMonkeyStudio.h>
#include <QueuedStatusBar.h>
#include <BuilderPlugin.h>
#include <CompilerPlugin.h>

#include <QApplication>
#include <QTextCodec>
#include <QFile>
#include <QDir>
#include <QFileInfo>

#include <QDebug>

QMakeProjectItem::QMakeProjectItem()
	: XUPProjectItem()
{
}

QMakeProjectItem::~QMakeProjectItem()
{
}

QString QMakeProjectItem::interpretVariable( const QString& variableName, const XUPItem* callerItem, const QString& defaultValue ) const
{
	/*
		$$[QT_INSTALL_HEADERS] : read content from qt conf
		$${QT_INSTALL_HEADERS} or $$QT_INSTALL_HEADERS : read content from var
		$$(QT_INSTALL_HEADERS) : read from environment when qmake run
		$(QTDIR) : read from generated makefile
	*/
	
	QString name = QString( variableName ).replace( '$', "" ).replace( '{', "" ).replace( '}', "" ).replace( '[', "" ).replace( ']', "" ).replace( '(', "" ).replace( ')', "" );
	QList<QStringList> value;
	
	// environment var
	if ( variableName.startsWith( "$$(" ) || name == "PWD" )
	{
		return name != "PWD" ? qgetenv( name.toLocal8Bit().constData() ) : ( callerItem ? callerItem->project()->path() : path() );
	}
	else if ( variableName.startsWith( "$(" ) )
	{
		const_cast<QMakeProjectItem*>( this )->setLastError( tr( QT_TR_NOOP( "Don't know how to interpret: %1" ) ).arg( variableName ) );
	}
	else if ( variableName.startsWith( "$$[" ) )
	{
		const_cast<QMakeProjectItem*>( this )->setLastError( tr( QT_TR_NOOP( "Don't know how to interpret: %1" ) ).arg( variableName ) );
	}
	else
	{
		QList<XUPItem*> variableItems = getVariables( this, name, callerItem );
		foreach ( XUPItem* variableItem, variableItems )
		{
			const QString op = variableItem->attribute( "operator", "=" );
			QStringList tmp;
			for ( int i = 0; i < variableItem->childCount(); i++ )
			{
				XUPItem* valueItem = variableItem->child( i );
				if ( valueItem->type() == XUPItem::Value ||
					valueItem->type() == XUPItem::File ||
					valueItem->type() == XUPItem::Path
				)
				{
					tmp << interpretValue( valueItem, "content" );
				}
			}
			
			if ( op == "=" )
			{
				value = QList<QStringList>() << QStringList( tmp );
			}
			else if ( op == "-=" )
			{
				value.removeAll( tmp );
			}
			else if ( op == "+=" )
			{
				value << tmp;
			}
			else if ( op == "*=" )
			{
				if ( !value.contains( tmp ) )
					value << tmp;
			}
			else if ( op == "~=" )
			{
				const_cast<QMakeProjectItem*>( this )->setLastError( tr( QT_TR_NOOP( "Don't know how to interpret ~= operator" ) ) );
			}
		}
	}
	
	QStringList result;
	foreach ( const QStringList& values, value )
		result << values;
	
	return result.isEmpty() ? defaultValue : result.join( " " );
}

QString QMakeProjectItem::interpretValue( XUPItem* callerItem, const QString& attribute ) const
{
	QRegExp rx( "\\$\\$?[\\{\\(\\[]?(\\w+(?!\\w*\\s*[()]))[\\}\\)\\]]?" );
	const QString content = callerItem->attribute( attribute );
	QString value = content;
	int pos = 0;
	while ( ( pos = rx.indexIn( content, pos ) ) != -1 )
	{
		value.replace( rx.cap( 0 ), interpretVariable( rx.cap( 0 ), callerItem ) );
		pos += rx.matchedLength();
	}
	return value;
}

QString QMakeProjectItem::toString() const
{
	return QMake2XUP::convertToPro( mDocument );
}

void QMakeProjectItem::registerProjectType() const
{
	// get proejct type
	int pType = projectType();
	
	// register it
	mXUPProjectInfos->unRegisterType( pType );
	mXUPProjectInfos->registerType( pType, const_cast<QMakeProjectItem*>( this ) );
		
	// values
	const QString mPixmapsPath = ":/qmakeitems";
	const QStringList mOperators = QStringList( "=" ) << "+=" << "-=" << "*=" << "~=";
	const QStringList mFilteredVariables = QStringList() << "FORMS" << "FORMS3"
		<< "HEADERS" << "SOURCES" << "OBJECTIVE_SOURCES" << "YACCSOURCES" << "LEXSOURCES"
		<< "TRANSLATIONS" << "RESOURCES" << "RC_FILE" << "RES_FILE" << "DEF_FILE"
		<< "INCLUDEPATH" << "DEPENDPATH" << "VPATH" << "LIBS" << "DEFINES" << "OTHER_FILES";
	const QStringList mFileVariables = QStringList( "FORMS" ) << "FORMS3" << "HEADERS"
		<< "SOURCES" << "OBJECTIVE_SOURCES" << "YACCSOURCES" << "LEXSOURCES"
		<< "TRANSLATIONS" << "RESOURCES" << "RC_FILE" << "RES_FILE" << "DEF_FILE" << "SUBDIRS" << "OTHER_FILES";
	const QStringList mPathVariables = QStringList( "INCLUDEPATH" ) << "DEPENDPATH"
		<< "VPATH";
	const StringStringListList mSuffixes = StringStringListList()
		<< qMakePair( tr( QT_TR_NOOP( "Qt Project" ) ), QStringList( "*.pro" ) )
		<< qMakePair( tr( QT_TR_NOOP( "Qt Include Project" ) ), QStringList( "*.pri" ) );
	const StringStringList mVariableLabels = StringStringList()
		<< qMakePair( QString( "FORMS" ), tr( QT_TR_NOOP( "Forms Files" ) ) )
		<< qMakePair( QString( "FORMS3" ), tr( QT_TR_NOOP( "Forms 3 Files" ) ) )
		<< qMakePair( QString( "HEADERS" ), tr( QT_TR_NOOP( "Headers Files" ) ) )
		<< qMakePair( QString( "SOURCES" ), tr( QT_TR_NOOP( "Sources Files" ) ) )
		<< qMakePair( QString( "OBJECTIVE_SOURCES" ), tr( QT_TR_NOOP( "Objective Sources Files" ) ) )
		<< qMakePair( QString( "TRANSLATIONS" ), tr( QT_TR_NOOP( "Qt Translations Files" ) ) )
		<< qMakePair( QString( "RESOURCES" ), tr( QT_TR_NOOP( "Qt Resources Files" ) ) )
		<< qMakePair( QString( "RC_FILE" ), tr( QT_TR_NOOP( "Resources Files" ) ) )
		<< qMakePair( QString( "RES_FILE" ), tr( QT_TR_NOOP( "Compiled Resources Files" ) ) )
		<< qMakePair( QString( "DEF_FILE" ), tr( QT_TR_NOOP( "Definitions Files" ) ) )
		<< qMakePair( QString( "SUBDIRS" ), tr( QT_TR_NOOP( "Sub Projects" ) ) )
		<< qMakePair( QString( "INCLUDEPATH" ), tr( QT_TR_NOOP( "Includes Paths" ) ) )
		<< qMakePair( QString( "DEPENDPATH" ), tr( QT_TR_NOOP( "Depends Paths" ) ) )
		<< qMakePair( QString( "VPATH" ), tr( QT_TR_NOOP( "Virtuals Paths" ) ) )
		<< qMakePair( QString( "LIBS" ), tr( QT_TR_NOOP( "Libraries Files" ) ) )
		<< qMakePair( QString( "DEFINES" ), tr( QT_TR_NOOP( "Defines" ) ) )
		<< qMakePair( QString( "OTHER_FILES" ), tr( QT_TR_NOOP( "Other Files" ) ) );
	const StringStringList mVariableIcons = StringStringList()
		<< qMakePair( QString( "FORMS" ), QString( "forms" ) )
		<< qMakePair( QString( "FORMS3" ), QString( "forms" ) )
		<< qMakePair( QString( "HEADERS" ), QString( "headers" ) )
		<< qMakePair( QString( "SOURCES" ), QString( "sources" ) )
		<< qMakePair( QString( "OBJECTIVE_SOURCES" ), QString( "objective_sources" ) )
		<< qMakePair( QString( "TRANSLATIONS" ), QString( "translations" ) )
		<< qMakePair( QString( "RESOURCES" ), QString( "resources" ) )
		<< qMakePair( QString( "RC_FILE" ), QString( "rc_file" ) )
		<< qMakePair( QString( "RES_FILE" ), QString( "res_file" ) )
		<< qMakePair( QString( "DEF_FILE" ), QString( "def_file" ) )
		<< qMakePair( QString( "SUBDIRS" ), QString( "project" ) )
		<< qMakePair( QString( "INCLUDEPATH" ), QString( "includepath" ) )
		<< qMakePair( QString( "DEPENDPATH" ), QString( "dependpath" ) )
		<< qMakePair( QString( "VPATH" ), QString( "vpath" ) )
		<< qMakePair( QString( "LIBS" ), QString( "libs" ) )
		<< qMakePair( QString( "DEFINES" ), QString( "defines" ) )
		<< qMakePair( QString( "OTHER_FILES" ), QString( "file" ) );
	const QStringList cf = pMonkeyStudio::availableLanguagesSuffixes().value( "C++" );
	// HEADERS filters
	QStringList hf;
	foreach ( QString f, cf )
		if ( f.startsWith( "*.h", Qt::CaseInsensitive ) )
			hf << f;
	// SOURCES filters
	QStringList sf;
	foreach ( QString f, cf )
		if ( f.startsWith( "*.c", Qt::CaseInsensitive ) )
			sf << f;
	// YACC filters
	QStringList yf;
	foreach ( QString s, sf )
		if ( !yf.contains( s.replace( "c", "y", Qt::CaseInsensitive ) ) )
			yf << s;
	// LEX filters
	QStringList lf;
	foreach ( QString s, sf )
		if ( s.startsWith( "*.c", Qt::CaseInsensitive ) && !lf.contains( s.replace( "c", "l", Qt::CaseInsensitive ) ) )
			lf << s;
	// PROJECT filters
	QStringList pjf;
	foreach ( const PairStringStringList& p, mSuffixes )
		pjf << p.second;
	// Variable suffixes
	const StringStringListList mVariableSuffixes = StringStringListList()
		<< qMakePair( QString( "HEADERS" ), hf )
		<< qMakePair( QString( "SOURCES" ), sf )
		<< qMakePair( QString( "YACCSOURCES" ), yf )
		<< qMakePair( QString( "LEXSOURCES" ), lf )
		<< qMakePair( QString( "OBJECTIVE_SOURCES" ), QStringList( "*.m" ) << "*.mm" )
		<< qMakePair( QString( "FORMS" ), QStringList( "*.ui" ) )
		<< qMakePair( QString( "FORMS3" ), QStringList( "*.ui" ) )
		<< qMakePair( QString( "TRANSLATIONS" ), QStringList( "*.ts" ) )
		<< qMakePair( QString( "RESOURCES" ), QStringList( "*.qrc" ) )
		<< qMakePair( QString( "DEF_FILE" ), QStringList( "*.def" ) )
		<< qMakePair( QString( "RC_FILE" ), QStringList( "*.rc" ) )
		<< qMakePair( QString( "RES_FILE" ), QStringList( "*.res" ) );
	
	// register values
	mXUPProjectInfos->registerPixmapsPath( pType, mPixmapsPath );
	mXUPProjectInfos->registerOperators( pType, mOperators );
	mXUPProjectInfos->registerFilteredVariables( pType, mFilteredVariables );
	mXUPProjectInfos->registerFileVariables( pType, mFileVariables );
	mXUPProjectInfos->registerPathVariables( pType, mPathVariables );
	mXUPProjectInfos->registerSuffixes( pType, mSuffixes );
	mXUPProjectInfos->registerVariableLabels( pType, mVariableLabels );
	mXUPProjectInfos->registerVariableIcons( pType, mVariableIcons );
	mXUPProjectInfos->registerVariableSuffixes( pType, mVariableSuffixes );
}

void QMakeProjectItem::customRowCount( XUPItem* item ) const
{
	if ( item->type() == XUPItem::Variable && item->attribute( "name" ) == "SUBDIRS" )
	{
		if ( !item->temporaryValue( "subdirsHandled", false ).toBool() )
		{
			item->setTemporaryValue( "subdirsHandled", true );
			QStringList subdirs;
			
			for ( int i = 0; i < item->childCount(); i++ )
			{
				XUPItem* cit = item->child( i );
				if ( cit->type() == XUPItem::File )
				{
					subdirs << splitMultiLineValue( cit->attribute( "content" ) );
				}
			}
			
			if ( !subdirs.isEmpty() )
			{
				foreach ( const QString& subdir, subdirs )
				{
					// generate project filepath
					QString fn = subdir;
					if ( QDir::isRelativePath( subdir ) )
						fn = filePath( subdir );
					QFileInfo fi( fn );
					if ( fi.isDir() )
						fi.setFile( fn, QString( "%1.pro" ).arg( subdir ) );
					// open project
					XUPProjectItem* project = newProject();
					if ( project->open( fi.absoluteFilePath(), temporaryValue( "codec" ).toString() ) )
					{
						item->addChild( project );
					}
					else
					{
						delete project;
					}
				}
			}
		}
	}
}

bool QMakeProjectItem::open( const QString& fileName, const QString& codec )
{
	QString buffer = QMake2XUP::convertFromPro( fileName, codec );
	
	// parse content
	QString errorMsg;
	int errorLine;
	int errorColumn;
	if ( !mDocument.setContent( buffer, &errorMsg, &errorLine, &errorColumn ) )
	{
		setLastError( QString( "%1 on line: %2, column: %3" ).arg( errorMsg ).arg( errorLine ).arg( errorColumn ) );
		return false;
	}
	
	// check project validity
	mDomElement = mDocument.firstChildElement( "project" );
	if ( mDomElement.isNull() )
	{
		setLastError("no project node" );
		return false;
	}
	
	// all is ok
	setTemporaryValue( "codec", codec );
	setTemporaryValue( "fileName", fileName );
	setLastError( QString::null );
	
	return true;
}

bool QMakeProjectItem::save()
{
	return XUPProjectItem::save();
	
	/*
	setLastError( tr( "Save project: Not yet implemented\n" ) +toString() );
	return false;
	*/
}

BuilderPlugin* QMakeProjectItem::builder( const QString& plugin ) const
{
	QString plug = plugin;
	
	if ( plug.isEmpty() )
	{
		QtVersionManager mQtManager;
		QtVersion mQtVersion = mQtManager.version( projectSettingsValue( "QT_VERSION" ) );
		
		if ( mQtVersion.isValid() )
		{
			if ( mQtVersion.QMakeSpec.contains( "msvc", Qt::CaseInsensitive ) )
			{
				plug = "MSVCMake";
			}
		}
		
		if ( plug.isEmpty() )
		{
			plug = "GNUMake";
		}
	}
	
	return XUPProjectItem::builder( plug );
}

CompilerPlugin* QMakeProjectItem::compiler( const QString& plugin ) const
{
	QString plug = plugin;
	
	if ( plug.isEmpty() )
	{
		QtVersionManager mQtManager;
		QtVersion mQtVersion = mQtManager.version( projectSettingsValue( "QT_VERSION" ) );
		
		if ( mQtVersion.isValid() )
		{
			if ( mQtVersion.QMakeSpec.contains( "msvc", Qt::CaseInsensitive ) )
			{
				plug = "MSVC";
			}
		}
		
		if ( plug.isEmpty() )
		{
			plug = "G++";
		}
	}
	
	return XUPProjectItem::compiler( plug );
}

DebuggerPlugin* QMakeProjectItem::debugger( const QString& plugin ) const
{
	QString plug = plugin;
	
	if ( plug.isEmpty() )
	{
		QtVersionManager mQtManager;
		QtVersion mQtVersion = mQtManager.version( projectSettingsValue( "QT_VERSION" ) );
		
		if ( mQtVersion.isValid() )
		{
			if ( !mQtVersion.QMakeSpec.contains( "msvc", Qt::CaseInsensitive ) )
			{
				plug = "GNUDebugger";
			}
		}
	}
	
	return XUPProjectItem::debugger( plug );
}

InterpreterPlugin* QMakeProjectItem::interpreter( const QString& plugin ) const
{
	QString plug = plugin;
	
	if ( plug.isEmpty() )
	{
		/*
		QtVersionManager mQtManager;
		QtVersion mQtVersion = mQtManager.version( projectSettingsValue( "QT_VERSION" ) );
		
		if ( mQtVersion.isValid() )
		{
			if ( mQtVersion.QMakeSpec.contains( "msvc", Qt::CaseInsensitive ) )
			{
				plug = "MSVC";
			}
		}
		
		if ( plug.isEmpty() )
		{
			plug = "G++";
		}
		*/
	}
	
	return XUPProjectItem::interpreter( plug );
}

void QMakeProjectItem::installCommands()
{
	// get plugins
	BuilderPlugin* bp = builder();
	CompilerPlugin* cp = compiler();
	
	// temp command
	pCommand cmd;
	
	// build command
	if ( bp )
	{
		cmd = bp->buildCommand();
		
		if ( cp )
		{
			cmd.addParsers( cp->compileCommand().parsers() );
		}
	}
	
	cmd.setUserData( QVariant::fromValue( &mCommands ) );
	cmd.setProject( this );
	cmd.setSkipOnError( false );
	const pCommand cmdBuild = cmd;
	
	// get qt version
	QtVersionManager mQtManager;
	QtVersion mQtVersion = mQtManager.version( projectSettingsValue( "QT_VERSION" ) );
	
	// evaluate some variables
	QString s;
	s = rootIncludeProject()->interpretVariable( "TARGET" );
	
	if ( s.isEmpty() )
	{
		s = QFileInfo( fileName() ).baseName();
	
	}
	
	const QString target = s;
	s = rootIncludeProject()->interpretVariable( "DESTDIR" );
	
	if ( s.isEmpty() )
	{
		s = rootIncludeProject()->interpretVariable( "DLLDESTDIR" );
	}
	
	if ( QDir( s ).isRelative() )
	{
		s.prepend( "$cpp$/" );
	}
	
	if ( s.endsWith( '/' ) )
	{
		s.chop( 1 );
	}
	
	const QString destdir = s;
	
	// compiler
	if ( bp && cmdBuild.isValid() )
	{
		// build debug
		cmd = cmdBuild;
		cmd.setText( tr( "Build Debug" ) );
		cmd.setArguments( "debug" );
		addCommand( cmd, "mBuilder/mBuild" );
		
		// build release
		cmd = cmdBuild;
		cmd.setText( tr( "Build Release" ) );
		cmd.setArguments( "release" );
		addCommand( cmd, "mBuilder/mBuild" );
		
		// build all
		cmd = cmdBuild;
		cmd.setText( tr( "Build All" ) );
		cmd.setArguments( "all" );
		addCommand( cmd, "mBuilder/mBuild" );
		
		// clean
		cmd = cmdBuild;
		cmd.setText( tr( "Clean" ) );
		cmd.setArguments( "clean" );
		addCommand( cmd, "mBuilder/mClean" );
		
		// distclean
		cmd = cmdBuild;
		cmd.setText( tr( "Distclean" ) );
		cmd.setArguments( "distclean" );
		addCommand( cmd, "mBuilder/mClean" );
		
		// add qt commands only if possible
		if ( mQtVersion.isValid() )
		{
			// qmake command
			cmd = pCommand();
			cmd.setText( tr( "QMake" ) );
			cmd.setCommand( mQtVersion.qmake() );
			cmd.setArguments( mQtVersion.qmakeParameters()/*.append( " $cp$" )*/ );
			cmd.setWorkingDirectory( "$cpp$" );
			cmd.setUserData( QVariant::fromValue( &mCommands ) );
			cmd.setProject( this );
			cmd.setSkipOnError( false );
			addCommand( cmd, "mBuilder" );
			
			// lupdate command
			cmd = pCommand();
			cmd.setText( tr( "lupdate" ) );
			cmd.setCommand( mQtVersion.lupdate() );
			cmd.setArguments( "$cp$" );
			cmd.setWorkingDirectory( "$cpp$" );
			cmd.setUserData( QVariant::fromValue( &mCommands ) );
			cmd.setProject( this );
			cmd.setSkipOnError( false );
			addCommand( cmd, "mBuilder" );
			
			// lrelease command
			cmd = pCommand();
			cmd.setText( tr( "lrelease" ) );
			cmd.setCommand( mQtVersion.lrelease() );
			cmd.setArguments( "$cp$" );
			cmd.setWorkingDirectory( "$cpp$" );
			cmd.setUserData( QVariant::fromValue( &mCommands ) );
			cmd.setProject( this );
			cmd.setSkipOnError( false );
			addCommand( cmd, "mBuilder" );
		
			// rebuild debug
			cmd = cmdBuild;
			cmd.setText( tr( "Rebuild Debug" ) );
			cmd.setCommand( ( QStringList() << tr( "QMake" ) << tr( "Distclean" ) << tr( "QMake" ) << tr( "Build Debug" ) ).join( ";" ) );
			cmd.setArguments( QString() );
			addCommand( cmd, "mBuilder/mRebuild" );
			
			// rebuild release
			cmd = cmdBuild;
			cmd.setText( tr( "Rebuild Release" ) );
			cmd.setCommand( ( QStringList() << tr( "QMake" ) << tr( "Distclean" ) << tr( "QMake" ) << tr( "Build Release" ) ).join( ";" ) );
			cmd.setArguments( QString() );
			addCommand( cmd, "mBuilder/mRebuild" );
			
			// rebuild all
			cmd = cmdBuild;
			cmd.setText( tr( "Rebuild All" ) );
			cmd.setCommand( ( QStringList() << tr( "QMake" ) << tr( "Distclean" ) << tr( "QMake" ) << tr( "Build All" ) ).join( ";" ) );
			cmd.setArguments( QString() );
			addCommand( cmd, "mBuilder/mRebuild" );
			
			// simple rebuild call
			cmd = cmdBuild;
			cmd.setText( tr( "Rebuild" ) );
			cmd.setCommand( ( QStringList() << tr( "QMake" ) << tr( "Distclean" ) << tr( "QMake" ) << tr( "Build" ) ).join( ";" ) );
			cmd.setArguments( QString() );
			addCommand( cmd, "mBuilder/mRebuild" );
		}
		else if ( projectSettingsValue( "SHOW_QT_VERSION_WARNING", "1" ) == "1" )
		{
			setProjectSettingsValue( "SHOW_QT_VERSION_WARNING", "0" );
			topLevelProject()->save();
			MonkeyCore::statusBar()->appendMessage( tr( "Some actions can't be created, because there is no default Qt version setted, please go in your project settings ( %1 ) to fix this." ).arg( displayText() ) );
		}
		
		// execute debug
		cmd = cmdBuild;
		cmd.setText( tr( "Execute Debug" ) );
		cmd.setCommand( target );
		cmd.setArguments( QString() );
		cmd.setWorkingDirectory( destdir != "$cpp$" ? destdir : destdir +"/debug" );
		addCommand( cmd, "mBuilder/mExecute" );
		
		// execute release
		cmd = cmdBuild;
		cmd.setText( tr( "Execute Release" ) );
		cmd.setCommand( target );
		cmd.setArguments( QString() );
		cmd.setWorkingDirectory( destdir != "$cpp$" ? destdir : destdir +"/release" );
		addCommand( cmd, "mBuilder/mExecute" );
	}
	
	// install defaults commands
	XUPProjectItem::installCommands();
}
