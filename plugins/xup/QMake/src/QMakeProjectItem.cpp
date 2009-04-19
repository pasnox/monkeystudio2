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
#include <QProcess>

#include <QDebug>

QMakeProjectItem::QMakeProjectItem()
	: XUPProjectItem()
{
}

QMakeProjectItem::~QMakeProjectItem()
{
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
		<< qMakePair( tr( "Qt Project" ), QStringList( "*.pro" ) )
		<< qMakePair( tr( "Qt Include Project" ), QStringList( "*.pri" ) );
	const StringStringList mVariableLabels = StringStringList()
		<< qMakePair( QString( "FORMS" ), tr( "Forms Files" ) )
		<< qMakePair( QString( "FORMS3" ), tr( "Forms 3 Files" ) )
		<< qMakePair( QString( "HEADERS" ), tr( "Headers Files" ) )
		<< qMakePair( QString( "SOURCES" ), tr( "Sources Files" ) )
		<< qMakePair( QString( "OBJECTIVE_SOURCES" ), tr( "Objective Sources Files" ) )
		<< qMakePair( QString( "TRANSLATIONS" ), tr( "Qt Translations Files" ) )
		<< qMakePair( QString( "RESOURCES" ), tr( "Qt Resources Files" ) )
		<< qMakePair( QString( "RC_FILE" ), tr( "Resources Files" ) )
		<< qMakePair( QString( "RES_FILE" ), tr( "Compiled Resources Files" ) )
		<< qMakePair( QString( "DEF_FILE" ), tr( "Definitions Files" ) )
		<< qMakePair( QString( "SUBDIRS" ), tr( "Sub Projects" ) )
		<< qMakePair( QString( "INCLUDEPATH" ), tr( "Includes Paths" ) )
		<< qMakePair( QString( "DEPENDPATH" ), tr( "Depends Paths" ) )
		<< qMakePair( QString( "VPATH" ), tr( "Virtuals Paths" ) )
		<< qMakePair( QString( "LIBS" ), tr( "Libraries Files" ) )
		<< qMakePair( QString( "DEFINES" ), tr( "Defines" ) )
		<< qMakePair( QString( "OTHER_FILES" ), tr( "Other Files" ) );
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
		<< qMakePair( QString( "RES_FILE" ), QStringList( "*.res" ) )
		<< qMakePair( QString( "SUBDIRS" ), QStringList( "*.pro" ) );
	
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

bool QMakeProjectItem::handleSubdirs( XUPItem* subdirs )
{
	QStringList projects;
	XUPProjectItem* proj = subdirs->project();
	
	foreach ( XUPItem* cit, subdirs->childrenList() )
	{
		if ( cit->type() == XUPItem::File )
		{
			QStringList cacheFns = splitMultiLineValue( cit->cacheValue( "content" ) );
			
			foreach ( QString cacheFn, cacheFns )
			{
				if ( cacheFn.isEmpty() )
				{
					continue;
				}
				
				QString fn = filePath( cacheFn );
				QFileInfo fi( fn );
				
				if ( cacheFn.endsWith( "/" ) )
				{
					cacheFn.chop( 1 );
				}
				
				int sepPos = cacheFn.lastIndexOf( "/" );
				
				if ( sepPos != -1 )
				{
					cacheFn = cacheFn.mid( sepPos +1 );
				}
				
				if ( fi.isDir() )
				{
					fi.setFile( fn, QString( "%1.pro" ).arg( cacheFn ) );
				}
				
				fn = fi.absoluteFilePath();
				
				if ( !projects.contains( fn ) )
				{
					projects << fn;
				}
			}
		}
	}
	
	foreach ( XUPItem* cit, proj->childrenList() )
	{
		if ( cit->type() == XUPItem::Project )
		{
			if ( projects.contains( cit->project()->fileName() ) )
			{
				projects.removeAll( cit->project()->fileName() );
			}
		}
	}
	
	foreach ( const QString& fn, projects )
	{
		// open project
		XUPProjectItem* project = newProject();
		proj->addChild( project );
		
		// remove and delete project if can't open
		if ( !project->open( fn, temporaryValue( "codec" ).toString() ) )
		{
			proj->removeChild( project );
			topLevelProject()->setLastError( tr( "Failed to handle subdirs file %1" ).arg( fn ) );
			return false;
		}
	}
	
	return true;
}

QString QMakeProjectItem::getVariableContent( const QString& variableName )
{
	/*
		$$[QT_INSTALL_HEADERS] : read content from qt conf
		$${QT_INSTALL_HEADERS} or $$QT_INSTALL_HEADERS : read content from var
		$$(QT_INSTALL_HEADERS) : read from environment when qmake run
		$(QTDIR) : read from generated makefile
	*/
	
	QString name = QString( variableName ).replace( '$', "" ).replace( '{', "" ).replace( '}', "" ).replace( '[', "" ).replace( ']', "" ).replace( '(', "" ).replace( ')', "" );
	
	// environment var
	if ( variableName.startsWith( "$$(" ) || variableName.startsWith( "$(" ) )
	{
		if ( name == "PWD" )
		{
			return rootIncludeProject()->path();
		}
		else
		{
			return QString::fromLocal8Bit( qgetenv( name.toLocal8Bit().constData() ) );
		}
	}
	else if ( variableName.startsWith( "$$[" ) )
	{
		XUPProjectItem* proj = rootIncludeProject();
		
		if ( proj->variableCache().contains( name ) )
		{
			return proj->variableCache().value( name );
		}
		
		QString result;
		QtVersionManager mQtManager;
		QtVersion mQtVersion = mQtManager.version( projectSettingsValue( "QT_VERSION" ) );
		
		if ( mQtVersion.isValid() )
		{
			QProcess query;
			query.start( QString( "%1 -query %2" ).arg( mQtVersion.qmake() ).arg( name ) );
			query.waitForFinished();
			QString result = QString::fromLocal8Bit( query.readAll() ).trimmed();
			
			if ( result == "**Unknown**" )
			{
				result.clear();
			}
		}
		
		//proj->variableCache()[ name ] = result;
		return result;
	}
	else
	{
		if ( name == "PWD" )
		{
			return project()->path();
		}
		else
		{
			return rootIncludeProject()->variableCache().value( name );
		}
	}
	
	return QString::null;
}

bool QMakeProjectItem::analyze( XUPItem* item )
{
	QStringList values;
	
	foreach ( XUPItem* cItem, item->childrenList() )
	{
		switch ( cItem->type() )
		{
			case XUPItem::Value:
			case XUPItem::File:
			case XUPItem::Path:
			{
				QString content = interpretContent( cItem->attribute( "content" ) );
				values << content;
				
				cItem->setCacheValue( "content", content );
				break;
			}
			case XUPItem::Function:
			{
				QString parameters = interpretContent( cItem->attribute( "parameters" ) );
				
				cItem->setCacheValue( "parameters", parameters );
				break;
			}
			case XUPItem::Project:
			case XUPItem::Comment:
			case XUPItem::EmptyLine:
			case XUPItem::Variable:
			case XUPItem::Scope:
			case XUPItem::DynamicFolder:
			case XUPItem::Folder:
			default:
				break;
		}
		
		if ( !analyze( cItem ) )
		{
			return false;
		}
	}
	
	if ( item->type() == XUPItem::Variable )
	{
		QString name = item->attribute( "name" );
		QString op = item->attribute( "operator", "=" );
		XUPProjectItem* proj = rootIncludeProject();
		
		if ( op == "=" )
		{
			proj->variableCache()[ name ] = values.join( " " );
		}
		else if ( op == "-=" )
		{
			foreach ( const QString& value, values )
			{
				proj->variableCache()[ name ].replace( QRegExp( QString( "\\b%1\\b" ).arg( value ) ), QString::null );
			}
		}
		else if ( op == "+=" )
		{
			proj->variableCache()[ name ] += " " +values.join( " " );
		}
		else if ( op == "*=" )
		{
			//if ( !proj->variableCache()[ name ].contains( content ) )
			{
				proj->variableCache()[ name ] += " " +values.join( " " );
			}
		}
		else if ( op == "~=" )
		{
			topLevelProject()->setLastError( tr( "Don't know how to interpret ~= operator" ) );
		}
	}
	
	// handle include projects
	if ( item->attribute( "name" ).toLower() == "include" )
	{
		if ( !handleIncludeFile( item ) )
		{
			return false;
		}
	}
	
	// handle sub projects
	if ( item->attribute( "name" ) == "SUBDIRS" )
	{
		if ( !handleSubdirs( item ) )
		{
			return false;
		}
	}
	
	return true;
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
		topLevelProject()->setLastError( QString( "%1 on line: %2, column: %3" ).arg( errorMsg ).arg( errorLine ).arg( errorColumn ) );
		return false;
	}
	
	// check project validity
	mDomElement = mDocument.firstChildElement( "project" );
	if ( mDomElement.isNull() )
	{
		topLevelProject()->setLastError("no project node" );
		return false;
	}
	
	// all is ok
	setTemporaryValue( "codec", codec );
	setTemporaryValue( "fileName", fileName );
	topLevelProject()->setLastError( QString::null );
	
	return analyze( this );
}

bool QMakeProjectItem::save()
{
	return XUPProjectItem::save();
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
	
	// config variable
	XUPProjectItem* riProject = rootIncludeProject();
	QStringList config = splitMultiLineValue( riProject->variableCache().value( "CONFIG" ) );
	bool haveDebug = config.contains( "debug" );
	bool haveRelease = config.contains( "release" );
	bool haveDebugRelease = config.contains( "debug_and_release" );
	
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
	s = riProject->variableCache().value( "TARGET" );
	
	if ( s.isEmpty() )
	{
		s = QFileInfo( fileName() ).baseName();
	}
	
	const QString target = s;
	s = riProject->variableCache().value( "DESTDIR" );
	
	if ( s.isEmpty() )
	{
		s = riProject->variableCache().value( "DLLDESTDIR" );
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
		if ( haveDebug || haveDebugRelease )
		{
			cmd = cmdBuild;
			cmd.setText( tr( "Build Debug" ) );
			if ( haveDebugRelease )
			{
				cmd.setArguments( "debug" );
			}
			addCommand( cmd, "mBuilder/mBuild" );
		}
		
		// build release
		if ( haveRelease || haveDebugRelease )
		{
			cmd = cmdBuild;
			cmd.setText( tr( "Build Release" ) );
			if ( haveDebugRelease )
			{
				cmd.setArguments( "release" );
			}
			addCommand( cmd, "mBuilder/mBuild" );
		}
		
		// build all
		if ( haveDebugRelease )
		{
			cmd = cmdBuild;
			cmd.setText( tr( "Build All" ) );
			cmd.setArguments( "all" );
			addCommand( cmd, "mBuilder/mBuild" );
		}
		
		// clean debug
		if ( haveDebug || haveDebugRelease )
		{
			cmd = cmdBuild;
			cmd.setText( tr( "Clean Debug" ) );
			if ( haveDebugRelease )
			{
				cmd.setArguments( "debug-clean" );
			}
			else
			{
				cmd.setArguments( "clean" );
			}
			addCommand( cmd, "mBuilder/mClean" );
		}
		
		// clean release
		if ( haveRelease || haveDebugRelease )
		{
			cmd = cmdBuild;
			cmd.setText( tr( "Clean Release" ) );
			if ( haveDebugRelease )
			{
				cmd.setArguments( "release-clean" );
			}
			else
			{
				cmd.setArguments( "clean" );
			}
			addCommand( cmd, "mBuilder/mClean" );
		}
		
		// clean all
		if ( haveDebugRelease )
		{
			cmd = cmdBuild;
			cmd.setText( tr( "Clean All" ) );
			cmd.setArguments( "clean" );
			addCommand( cmd, "mBuilder/mClean" );
		}
		
		// distclean debug
		if ( haveDebug || haveDebugRelease )
		{
			cmd = cmdBuild;
			cmd.setText( tr( "Distclean Debug" ) );
			if ( haveDebugRelease )
			{
				cmd.setArguments( "debug-distclean" );
			}
			else
			{
				cmd.setArguments( "distclean" );
			}
			addCommand( cmd, "mBuilder/mClean" );
		}
		
		// distclean release
		if ( haveRelease || haveDebugRelease )
		{
			cmd = cmdBuild;
			cmd.setText( tr( "Distclean Release" ) );
			if ( haveDebugRelease )
			{
				cmd.setArguments( "release-distclean" );
			}
			else
			{
				cmd.setArguments( "distclean" );
			}
			addCommand( cmd, "mBuilder/mClean" );
		}
		
		// distclean all
		if ( haveDebugRelease )
		{
			cmd = cmdBuild;
			cmd.setText( tr( "Distclean All" ) );
			cmd.setArguments( "distclean" );
			addCommand( cmd, "mBuilder/mClean" );
		}
		
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
			if ( haveDebug || haveDebugRelease )
			{
				cmd = cmdBuild;
				cmd.setText( tr( "Rebuild Debug" ) );
				cmd.setCommand( ( QStringList() << tr( "QMake" ) << tr( "Distclean Debug" ) << tr( "QMake" ) << tr( "Build Debug" ) ).join( ";" ) );
				cmd.setArguments( QString() );
				addCommand( cmd, "mBuilder/mRebuild" );
			}
			
			// rebuild release
			if ( haveRelease || haveDebugRelease )
			{
				cmd = cmdBuild;
				cmd.setText( tr( "Rebuild Release" ) );
				cmd.setCommand( ( QStringList() << tr( "QMake" ) << tr( "Distclean Release" ) << tr( "QMake" ) << tr( "Build Release" ) ).join( ";" ) );
				cmd.setArguments( QString() );
				addCommand( cmd, "mBuilder/mRebuild" );
			}
			
			// rebuild all
			if ( haveDebugRelease )
			{
				cmd = cmdBuild;
				cmd.setText( tr( "Rebuild All" ) );
				cmd.setCommand( ( QStringList() << tr( "QMake" ) << tr( "Distclean All" ) << tr( "QMake" ) << tr( "Build All" ) ).join( ";" ) );
				cmd.setArguments( QString() );
				addCommand( cmd, "mBuilder/mRebuild" );
			}
		}
		else if ( projectSettingsValue( "SHOW_QT_VERSION_WARNING", "1" ) == "1" )
		{
			setProjectSettingsValue( "SHOW_QT_VERSION_WARNING", "0" );
			topLevelProject()->save();
			MonkeyCore::statusBar()->appendMessage( tr( "Some actions can't be created, because there is no default Qt version setted, please go in your project settings ( %1 ) to fix this." ).arg( displayText() ) );
		}
		
		// execute debug
		if ( haveDebug || haveDebugRelease )
		{
			cmd = cmdBuild;
			cmd.setText( tr( "Execute Debug" ) );
			cmd.setCommand( target );
			cmd.setArguments( QString() );
			cmd.setWorkingDirectory( destdir != "$cpp$" ? destdir : destdir +"/debug" );
			addCommand( cmd, "mBuilder/mExecute" );
		}
		
		// execute release
		if ( haveRelease || haveDebugRelease )
		{
			cmd = cmdBuild;
			cmd.setText( tr( "Execute Release" ) );
			cmd.setCommand( target );
			cmd.setArguments( QString() );
			cmd.setWorkingDirectory( destdir != "$cpp$" ? destdir : destdir +"/release" );
			addCommand( cmd, "mBuilder/mExecute" );
		}
	}
	
	// install defaults commands
	XUPProjectItem::installCommands();
}
