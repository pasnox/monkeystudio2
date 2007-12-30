#include "QMakeProjectItem.h"
#include "ProjectItemModel.h"
#include "XUPManager.h"
#include "QMake2XUP.h"

#include <QIcon>

using namespace XUPManager;

QIcon getIcon( const QString& s )
{ return QIcon( QString( ":/icons/icons/%1.png" ).arg( s ) ); }

QMakeProjectItem::QMakeProjectItem( const QDomElement& e, const QString& s, bool b, ProjectItem* bit )
	: ProjectItem( e, s, b, bit )
{}

void QMakeProjectItem::registerItem()
{
	// registering...
	// variables based on files
	registerFileVariables( "FORMS" );
	registerFileVariables( "FORMS3" );
	registerFileVariables( "HEADERS" );
	registerFileVariables( "SOURCES" );
	registerFileVariables( "OBJECTIVE_SOURCES" );
	registerFileVariables( "TRANSLATIONS" );
	registerFileVariables( "RESOURCES" );
	registerFileVariables( "RC_FILE" );
	registerFileVariables( "RES_FILE" );
	registerFileVariables( "DEF_FILE" );
	registerFileVariables( "SUBDIRS" );
	// variables based on paths
	registerPathVariables( "INCLUDEPATH" );
	registerPathVariables( "DEPENDPATH" );
	registerPathVariables( "VPATH" );
	// variables labels
	registerVariableLabels( "FORMS", tr( "Forms Files" ) );
	registerVariableLabels( "FORMS3", tr( "Forms 3 Files" ) );
	registerVariableLabels( "HEADERS", tr( "Headers Files" ) );
	registerVariableLabels( "SOURCES", tr( "Sources Files" ) );
	registerVariableLabels( "OBJECTIVE_SOURCES", tr( "Objective Sources Files" ) );
	registerVariableLabels( "TRANSLATIONS", tr( "Qt Translations Files" ) );
	registerVariableLabels( "RESOURCES", tr( "Qt Resources Files" ) );
	registerVariableLabels( "RC_FILE", tr( "Resources Files" ) );
	registerVariableLabels( "RES_FILE", tr( "Compiled Resources Files" ) );
	registerVariableLabels( "DEF_FILE", tr( "Definitions Files" ) );
	registerVariableLabels( "SUBDIRS", tr( "Sub Projects" ) );
	registerVariableLabels( "INCLUDEPATH", tr( "Includes Paths" ) );
	registerVariableLabels( "DEPENDPATH", tr( "Depends Paths" ) );
	registerVariableLabels( "VPATH", tr( "Virtuals Paths" ) );
	registerVariableLabels( "LIBS", tr( "Libraries Files" ) );
	registerVariableLabels( "DEFINES", tr( "Defines" ) );
	// variables icons
	registerVariableIcons( "FORMS", getIcon( "forms" ) );
	registerVariableIcons( "FORMS3", getIcon( "forms3" ) );
	registerVariableIcons( "HEADERS", getIcon( "headers" ) );
	registerVariableIcons( "SOURCES", getIcon( "sources" ) );
	registerVariableIcons( "OBJECTIVE_SOURCES", getIcon( "objective_sources" ) );
	registerVariableIcons( "TRANSLATIONS", getIcon( "translations" ) );
	registerVariableIcons( "RESOURCES", getIcon( "resources" ) );
	registerVariableIcons( "RC_FILE", getIcon( "rc_file" ) );
	registerVariableIcons( "RES_FILE", getIcon( "res_file" ) );
	registerVariableIcons( "DEF_FILE", getIcon( "def_file" ) );
	registerVariableIcons( "SUBDIRS", getIcon( "project" ) );
	registerVariableIcons( "INCLUDEPATH", getIcon( "includepath" ) );
	registerVariableIcons( "DEPENDPATH", getIcon( "dependpath" ) );
	registerVariableIcons( "VPATH", getIcon( "vpath" ) );
	registerVariableIcons( "LIBS", getIcon( "libs" ) );
	registerVariableIcons( "DEFINES", getIcon( "defines" ) );
	//
	qWarning( qPrintable( tr( "QMakeProjectItem Registered" ) ) ); 
}

QStringList QMakeProjectItem::filteredVariables() const
{
	return QStringList() << "FORMS" << "FORMS3" << "HEADERS" << "SOURCES" << "OBJECTIVE_SOURCES" << "TRANSLATIONS"
		<< "RESOURCES" << "RC_FILE" << "RES_FILE" << "DEF_FILE" << "INCLUDEPATH" << "DEPENDPATH" << "VPATH" << "LIBS" << "DEFINES";
}

QMakeProjectItem* QMakeProjectItem::clone( bool b ) const
{ return b ? new QMakeProjectItem( domElement().cloneNode( false ).toElement(), projectFilePath(), modified()/*, buddy()*/ ) : new QMakeProjectItem; }

QString QMakeProjectItem::interpretedVariable( const QString& s, const ProjectItem* it, const QString& d ) const
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
		if ( ProjectItem* pi = /* buddy() ? buddy()->topLevelProject() : */ topLevelProject() )
		{
			foreach ( ProjectItem* cit, pi->children( true, false ) )
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
	return ProjectItem::interpretedVariable( s, it, d );
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
	foreach ( ProjectItem* it, children( true, true ) )
	{
		QString s;
		if ( it->isType( "variable" ) && it->defaultValue() == "SUBDIRS" )
		{
			foreach ( ProjectItem* cit, it->children( false, true ) )
			{
				s = cit->filePath();
				QFileInfo fi( s );
				if ( fi.isDir() )
					s.append( QString( "/%1.pro" ).arg( fi.fileName() ) );
				
				if ( QFile::exists( s ) )
				{
					QMakeProjectItem* pi = new QMakeProjectItem;
					if ( pi->loadProject( s ) )
						it->parent()->appendRow( pi );
					else
						delete pi;
				}
			}
		}
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
	}
}

bool QMakeProjectItem::loadProject( const QString& s, const QString& v )
{
	if ( loadXUP( this, QMake2XUP::convertFromPro( s, v ), v ) )
	{
		mProjectFilePath = s;
		setModified( false );
		checkChildrenProjects();
		return true;
	}
	return false;
}

bool QMakeProjectItem::saveProject( const QString& s, const QString& v )
{
	if ( QMake2XUP::convertToPro( toDomDocument(), s.isEmpty() ? projectFilePath() : s, v ) )
	{
		mProjectFilePath = s.isEmpty() ? projectFilePath() : s;
		setModified( false );
		return true;
	}
	return false;
}

QString QMakeProjectItem::filePath( const QString& s )
{
	if ( s.isEmpty() && isType( "value" ) )
	{
		const QString v = parent()->defaultValue();
		if ( ( XUPManager::fileVariables().contains( v ) || XUPManager::pathVariables().contains( v ) ) && !defaultValue().isEmpty() )
		{
			const QString pp = projectPath().append( "/%1" );
			const QString iv = defaultInterpretedValue();
			if ( QFile::exists( pp.arg( iv ) ) )
				return QFileInfo( pp.arg( iv ) ).canonicalFilePath();
			
			// looking in paths
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
	return ProjectItem::filePath( s );
}

QStringList QMakeProjectItem::splitFiles( const QString& s ) const
{
	QStringList l;
	QRegExp rx = QRegExp( "(?:\"[^\"]+\")|([^ \"]+)" );
	int p = 0;
	while ( ( p = rx.indexIn( s, p ) ) != -1 )
	{
		l << rx.capturedTexts().value( 0 ).replace( '"', "" ).trimmed();
		p += rx.matchedLength();
	}
	return l;
}
