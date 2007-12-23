#include "QMakeProjectItem.h"
#include "ProjectItemModel.h"
#include "XUPManager.h"
#include "QMake2XUP.h"

#include <QIcon>

using namespace XUPManager;

QIcon getIcon( const QString& s )
{ return QIcon( QString( ":/icons/icons/%1.png" ).arg( s ) ); }

QMakeProjectItem::QMakeProjectItem( const QDomElement& e, const QString& s, bool b )
	: ProjectItem( e, s, b )
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
	registerVariableLabels( "TRANSLATIONS", tr( "Translations Files" ) );
	registerVariableLabels( "RESOURCES", tr( "Resources Files" ) );
	registerVariableLabels( "SUBDIRS", tr( "Sub Projects" ) );
	// variables labels
	registerVariableLabels( "INCLUDEPATH", tr( "Includes Paths" ) );
	registerVariableLabels( "DEPENDPATH", tr( "Depends Paths" ) );
	registerVariableLabels( "VPATH", tr( "Virtuals Paths" ) );
	registerVariableLabels( "LIBS", tr( "Libraries Files" ) );
	registerVariableLabels( "DEFINES", tr( "Defines" ) );
	registerVariableLabels( "RC_FILE", tr( "Resources Files" ) );
	registerVariableLabels( "RES_FILE", tr( "Compiled Resources Files" ) );
	registerVariableLabels( "DEF_FILE", tr( "Definitions Files" ) );
	// variables icons
	registerVariableIcons( "FORMS", getIcon( "forms" ) );
	registerVariableIcons( "FORMS3", getIcon( "forms3" ) );
	registerVariableIcons( "HEADERS", getIcon( "headers" ) );
	registerVariableIcons( "SOURCES", getIcon( "sources" ) );
	registerVariableIcons( "OBJECTIVE_SOURCES", getIcon( "objective_sources" ) );
	registerVariableIcons( "TRANSLATIONS", getIcon( "translations" ) );
	registerVariableIcons( "RESOURCES", getIcon( "resources" ) );
	registerVariableIcons( "SUBDIRS", getIcon( "project" ) );
	// variables icons
	registerVariableIcons( "INCLUDEPATH", getIcon( "includepath" ) );
	registerVariableIcons( "DEPENDPATH", getIcon( "dependpath" ) );
	registerVariableIcons( "VPATH", getIcon( "vpath" ) );
	registerVariableIcons( "LIBS", getIcon( "libs" ) );
	registerVariableIcons( "DEFINES", getIcon( "defines" ) );
	registerVariableIcons( "RC_FILE", getIcon( "rc_file" ) );
	registerVariableIcons( "RES_FILE", getIcon( "res_file" ) );
	registerVariableIcons( "DEF_FILE", getIcon( "def_file" ) );
	//
	qWarning( qPrintable( tr( "QMakeProjectItem Registered" ) ) ); 
}

QMakeProjectItem* QMakeProjectItem::clone( bool b ) const
{ return b ? new QMakeProjectItem( domElement(), projectFilePath(), modified() ) : new QMakeProjectItem; }

void QMakeProjectItem::appendRow( QMakeProjectItem* it )
{ ProjectItem::appendRow( it ); }

QString QMakeProjectItem::interpretedVariable( const QString& s, const ProjectItem* it, const QString& ) const
{
	if ( !model() )
		return QString();
	qWarning( "test: %s", qPrintable( s ) );
	move toplevel to item based members
	QString v;
	if ( ProjectItem* pi = model()->topLevelProject( this ) )
	{
		foreach ( ProjectItem* cit, pi->children( true, false ) )
		{
			if ( it && cit == it )
				return v;
			if ( model()->isType( cit, "variable" ) && cit->defaultValue() == s )
			{
				if ( cit->hasChildren() )
				{
					const QString o = cit->value( "operator", "=" );
					const QString cv = cit->child( 0 )->defaultValue();
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
						// don t know how to do ;)
					}
				}
			}
		}
	}
	return v;
}

QString QMakeProjectItem::defaultInterpretedValue() const
{
	QRegExp rx( "\\$\\$\\{?(\\w+)\\}?" );
	QString v = defaultValue();
	int p = 0;
	while ( ( p = rx.indexIn( v, p ) ) != -1 )
	{
		QString g = interpretedVariable( rx.capturedTexts().value( 1 ), this );
		qWarning( "%s: %s", qPrintable( rx.capturedTexts().value( 1 ) ), qPrintable( g ) );
		p += rx.matchedLength();
	}
	return ProjectItem::defaultInterpretedValue();
}

void QMakeProjectItem::checkChildrenProjects()
{
	foreach ( ProjectItem* it, children( true, true ) )
	{
		QString s;
		if ( it->value( "type" ) == "variable" && it->value( "name" ) == "SUBDIRS" )
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
		else if ( it->value( "type" ) == "function" && it->defaultValue().startsWith( "include" ) )
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
					delete pi;
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
