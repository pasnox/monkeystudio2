#include "QMakeProjectItem.h"
#include "ProjectItemModel.h"
#include "XUPManager.h"
#include "QMake2XUP.h"

#include <QFileInfo>
#include <QApplication>
#include <QDebug>

QMakeProjectItem::QMakeProjectItem( const QDomElement& e, const QString& s, bool b )
	: XUPItem()
{
	registerItem();
	setDomElement( e );
	loadProject( s );
	setModified( b );
}

void QMakeProjectItem::registerItem()
{
	// clearing default values
	mFilteredVariables.clear();
	//mTextTypes.clear();
	mFileVariables.clear();
	mPathVariables.clear();
	mVariableLabels.clear();
	//mVariableIcons.clear();
	//mOperators.clear();
	mSuffixes.clear();
	// registering...
	// filtered variables
	mFilteredVariables
		<< "FORMS"
		<< "FORMS3"
		<< "HEADERS"
		<< "SOURCES"
		<< "OBJECTIVE_SOURCES"
		<< "TRANSLATIONS"
		<< "RESOURCES"
		<< "RC_FILE"
		<< "RES_FILE"
		<< "DEF_FILE"
		<< "INCLUDEPATH"
		<< "DEPENDPATH"
		<< "VPATH"
		<< "LIBS"
		<< "DEFINES";
	// variables based on files
	mFileVariables
		<< "FORMS"
		<< "FORMS3"
		<< "HEADERS"
		<< "SOURCES"
		<< "OBJECTIVE_SOURCES"
		<< "TRANSLATIONS"
		<< "RESOURCES"
		<< "RC_FILE"
		<< "RES_FILE"
		<< "DEF_FILE"
		<< "SUBDIRS";
	// variables based on paths
	mPathVariables
		<< "INCLUDEPATH"
		<< "DEPENDPATH"
		<< "VPATH";
	// variables labels
	mVariableLabels[ "FORMS" ] = tr( "Forms Files" );
	mVariableLabels[ "FORMS3" ] = tr( "Forms 3 Files" );
	mVariableLabels[ "HEADERS" ] = tr( "Headers Files" );
	mVariableLabels[ "SOURCES" ] = tr( "Sources Files" );
	mVariableLabels[ "OBJECTIVE_SOURCES" ] = tr( "Objective Sources Files" );
	mVariableLabels[ "TRANSLATIONS" ] = tr( "Qt Translations Files" );
	mVariableLabels[ "RESOURCES" ] = tr( "Qt Resources Files" );
	mVariableLabels[ "RC_FILE" ] = tr( "Resources Files" );
	mVariableLabels[ "RES_FILE" ] = tr( "Compiled Resources Files" );
	mVariableLabels[ "DEF_FILE" ] = tr( "Definitions Files" );
	mVariableLabels[ "SUBDIRS" ] = tr( "Sub Projects" );
	mVariableLabels[ "INCLUDEPATH" ] = tr( "Includes Paths" );
	mVariableLabels[ "DEPENDPATH" ] = tr( "Depends Paths" );
	mVariableLabels[ "VPATH" ] = tr( "Virtuals Paths" );
	mVariableLabels[ "LIBS" ] = tr( "Libraries Files" );
	mVariableLabels[ "DEFINES" ] = tr( "Defines" );
	// variables icons
	mVariableIcons[ "FORMS" ] = getIcon( value( "icon" ), "forms" );
	mVariableIcons[ "FORMS3" ] = getIcon( value( "icon" ), "forms3" );
	mVariableIcons[ "HEADERS" ] = getIcon( value( "icon" ), "headers" );
	mVariableIcons[ "SOURCES" ] = getIcon( value( "icon" ), "sources" );
	mVariableIcons[ "OBJECTIVE_SOURCES" ] = getIcon( value( "icon" ), "objective_sources" );
	mVariableIcons[ "TRANSLATIONS" ] = getIcon( value( "icon" ), "translations" );
	mVariableIcons[ "RESOURCES" ] = getIcon( value( "icon" ), "resources" );
	mVariableIcons[ "RC_FILE" ] = getIcon( value( "icon" ), "rc_file" );
	mVariableIcons[ "RES_FILE" ] = getIcon( value( "icon" ), "res_file" );
	mVariableIcons[ "DEF_FILE" ] = getIcon( value( "icon" ), "def_file" );
	mVariableIcons[ "SUBDIRS" ] = getIcon( value( "icon" ), "project" );
	mVariableIcons[ "INCLUDEPATH" ] = getIcon( value( "icon" ), "includepath" );
	mVariableIcons[ "DEPENDPATH" ] = getIcon( value( "icon" ), "dependpath" );
	mVariableIcons[ "VPATH" ] = getIcon( value( "icon" ), "vpath" );
	mVariableIcons[ "LIBS" ] = getIcon( value( "icon" ), "libs" );
	mVariableIcons[ "DEFINES" ] = getIcon( value( "icon" ), "defines" );
	// register suffixes
	mSuffixes[tr( "Qt Project" )] = QStringList( "*.pro" );
	mSuffixes[tr( "Qt Project Include" )] = QStringList( "*.pri" );
}

QMakeProjectItem* QMakeProjectItem::clone( bool b ) const
{ return b ? new QMakeProjectItem( domElement(), projectFilePath(), modified() ) : new QMakeProjectItem; }

QString QMakeProjectItem::interpretedVariable( const QString& s, const XUPItem* it, const QString& d ) const
{
	/*
		$$[QT_INSTALL_HEADERS] : read content from qt conf
		$${QT_INSTALL_HEADERS} or $$QT_INSTALL_HEADERS : read content from var
		$$(QT_INSTALL_HEADERS) : read from environment when qmake run
		$(QTDIR) : read from generated makefile
	*/
	QString dv = QString( s ).replace( '$', "" ).replace( '{', "" ).replace( '}', "" ).replace( '[', "" ).replace( ']', "" ).replace( '(', "" ).replace( ')', "" );
	// environment var
	if ( s.startsWith( "$$(" ) || dv == QString( "PWD" ) )
		return dv != QString( "PWD" ) ? qgetenv( qPrintable( dv ) ) : projectPath();
	else if ( s.startsWith( "$(" ) )
		qWarning() << "Don't know how to proceed: " << s;
	else if ( s.startsWith( "$$[" ) )
		qWarning() << "Don't know how to proceed: " << s;
	else
	{
		QString v;
		if ( XUPItem* pi = topLevelProject() )
		{
			foreach ( XUPItem* cit, pi->children( true, false ) )
			{
				if ( it && cit == it )
					return v;
				if ( cit->isType( "variable" ) && cit->defaultValue() == dv )
				{
					if ( cit->hasChildren() )
					{
						const QString o = cit->value( "operator", "=" );
						const QString cv = cit->child( 0 )->defaultInterpretedValue();
						if ( o == "=" )
							v = cv;
						else if ( o == "-=" )
							v.remove( cv );
						else if ( o == "+=" )
							v.append( " " +cv );
						else if ( o == "*=" )
						{
							if ( !v.contains( cv ) )
								v.append( " " +cv );
						}
						else if ( o == "~=" )
						{
							qWarning() << "Don't know how to interpretate ~= operator";
						}
					}
				}
			}
		}
		return v;
	}
	return XUPItem::interpretedVariable( s, it, d );
}

QString QMakeProjectItem::defaultInterpretedValue() const
{
	QRegExp rx( "\\$\\$?[\\{\\(\\[]?(\\w+)[\\}\\)\\]]?" );
	const QString dv = defaultValue();
	QString v = dv;
	int p = 0;
	while ( ( p = rx.indexIn( dv, p ) ) != -1 )
	{
		v.replace( rx.capturedTexts().value( 0 ), interpretedVariable( rx.capturedTexts().value( 0 ), this ) );
		p += rx.matchedLength();
	}
	return v;
}

void QMakeProjectItem::checkChildrenProjects()
{
	foreach ( XUPItem* it, children( true, true ) )
	{
		QString fn;
		// get subdirs values
		if ( it->isType( "variable" ) && it->defaultValue() == "SUBDIRS" )
		{
			foreach ( XUPItem* cit, it->children( false, false ) )
			{
				// get filename
				fn = cit->filePath();
				QFileInfo fi( fn );
				if ( fi.isDir() )
					fn.append( QString( "/%1.pro" ).arg( fi.fileName() ) );
				// if file exists open project
				if ( QFile::exists( fn ) )
				{
					XUPItem* pi = clone( false );
					if ( pi->loadProject( fn ) )
						it->parent()->appendRow( pi );
					else
						delete pi;
				}
			}
		}
		/*
		else if ( it->isType( "function" ) && it->defaultValue().startsWith( "include" ) )
		{
			s = it->defaultInterpretedValue();
			s.remove( 0, s.indexOf( '(' ) +1 );
			s.chop( s.length() -s.lastIndexOf( ')' ) );
			s = it->filePath( s.trimmed() );
			
			if ( QFile::exists( s ) )
			{
				QMakeProjectItem* pi = new QMakeProjectItem;
				if ( pi->loadProject( s ) )
					it->appendRow( pi );
				else
				{
					delete pi;
					qWarning( "error loading include file: %s", qPrintable( s ) );
				}
			}
		}
		*/
	}
}

bool QMakeProjectItem::loadProject( const QString& s, const QString& v )
{
	if ( XUPManager::loadXUP( this, QMake2XUP::convertFromPro( s, v ), v ) )
	{
		mProjectFilePath = s;
		setModified( false );
		updateItem();
		checkChildrenProjects();
		QApplication::processEvents( QEventLoop::ExcludeUserInputEvents );
		return true;
	}
	return false;
}
#include <QTextEdit>

bool QMakeProjectItem::saveProject( const QString& s, const QString& v )
{
	return false;
	/*
	QByteArray a = QMake2XUP::convertToPro( toDomDocument(), v );
	if ( !a.isNull() )
	{
		QTextEdit* te = new QTextEdit;
		te->setPlainText( a );
		te->show();
		/
		QFile f( s.isEmpty() ? projectFilePath() : s );
		if ( !f.open( QIODevice::WriteOnly | QIODevice::Text ) )
			return false;
		f.resize( 0 );
		if ( !( f.write( a ) != -1 ) )
			return false;
		mProjectFilePath = f.fileName();
		/
		setModified( false );
		return true;
	}
	return false;
	*/
}

QString QMakeProjectItem::filePath( const QString& s )
{
	if ( s.isEmpty() && isType( "value" ) )
	{
		const QString v = parent()->defaultValue();
		if ( ( fileVariables().contains( v ) || pathVariables().contains( v ) ) && !defaultValue().isEmpty() )
		{
			const QString pp = projectPath().append( "/%1" );
			const QString iv = defaultInterpretedValue();
			if ( QFile::exists( pp.arg( iv ) ) )
				return QFileInfo( pp.arg( iv ) ).canonicalFilePath();
			
			// looking in paths of project
			QString ps = interpretedVariable( "INCLUDEPATH", this );
			ps += " " +interpretedVariable( "DEPENDPATH", this );
			ps += " " +interpretedVariable( "VPATH", this );
			foreach ( QString p, splitFiles( ps.trimmed() ) )
			{
				p.replace( '\\', '/' );
				if ( ( p.startsWith( '/' ) || p.mid( 1, 2 ) == ":/" ) && QFile::exists( QString( p ).append( "/%1" ).arg( iv ) ) )
					return QFileInfo( QString( p ).append( "/%1" ).arg( iv ) ).canonicalFilePath();
				else if ( QFile::exists( pp.arg( p ).append( "/%1" ).arg( iv ) ) )
					return QFileInfo( pp.arg( p ).append( "/%1" ).arg( iv ) ).canonicalFilePath();
			}
		}
	}
	return XUPItem::filePath( s );
}

QStringList QMakeProjectItem::splitFiles( const QString& s ) const
{
	QString fs = QString( s ).replace( projectPath(), "$pp$" );
	QStringList l;
	QRegExp rx = QRegExp( "(?:\"[^\"]+\")|([^ \"]+)" );
	int p = 0;
	while ( ( p = rx.indexIn( fs, p ) ) != -1 )
	{
		l << rx.capturedTexts().value( 0 ).replace( '"', "" ).trimmed().replace( "$pp$", projectPath() );
		p += rx.matchedLength();
	}
	return l;
}
