#include "QMakeProjectItem.h"
#include "ProjectItemModel.h"
#include "XUPManager.h"
#include "QMake2XUP.h"

#include <QFileInfo>
#include <QDebug>

QIcon getIcon( const QString& s )
{ return QIcon( QString( ":/icons/icons/%1.png" ).arg( s ) ); }

QMakeProjectItem::QMakeProjectItem( const QDomElement& e, const QString& s, bool b )
	: ProjectItem()
{
	registerItem();
	setDomElement( e );
	loadProject( s );
	setModified( b );
}

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
	registerVariableIcons( "FORMS", getIcon( value( "icon" ), "forms" ) );
	registerVariableIcons( "FORMS3", getIcon( value( "icon" ), "forms3" ) );
	registerVariableIcons( "HEADERS", getIcon( value( "icon" ), "headers" ) );
	registerVariableIcons( "SOURCES", getIcon( value( "icon" ), "sources" ) );
	registerVariableIcons( "OBJECTIVE_SOURCES", getIcon( value( "icon" ), "objective_sources" ) );
	registerVariableIcons( "TRANSLATIONS", getIcon( value( "icon" ), "translations" ) );
	registerVariableIcons( "RESOURCES", getIcon( value( "icon" ), "resources" ) );
	registerVariableIcons( "RC_FILE", getIcon( value( "icon" ), "rc_file" ) );
	registerVariableIcons( "RES_FILE", getIcon( value( "icon" ), "res_file" ) );
	registerVariableIcons( "DEF_FILE", getIcon( value( "icon" ), "def_file" ) );
	registerVariableIcons( "SUBDIRS", getIcon( value( "icon" ), "project" ) );
	registerVariableIcons( "INCLUDEPATH", getIcon( value( "icon" ), "includepath" ) );
	registerVariableIcons( "DEPENDPATH", getIcon( value( "icon" ), "dependpath" ) );
	registerVariableIcons( "VPATH", getIcon( value( "icon" ), "vpath" ) );
	registerVariableIcons( "LIBS", getIcon( value( "icon" ), "libs" ) );
	registerVariableIcons( "DEFINES", getIcon( value( "icon" ), "defines" ) );
	//
	qWarning( qPrintable( tr( "QMakeProjectItem Registered" ) ) ); 
}

QStringList QMakeProjectItem::filteredVariables() const
{
	return QStringList() << "FORMS" << "FORMS3" << "HEADERS" << "SOURCES" << "OBJECTIVE_SOURCES" << "TRANSLATIONS"
		<< "RESOURCES" << "RC_FILE" << "RES_FILE" << "DEF_FILE" << "INCLUDEPATH" << "DEPENDPATH" << "VPATH" << "LIBS" << "DEFINES";
}

QMakeProjectItem* QMakeProjectItem::clone( bool b ) const
{ return b ? new QMakeProjectItem( domElement(), projectFilePath(), modified() ) : new QMakeProjectItem; }

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
	return;
	
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
	if ( XUPManager::loadXUP( this, QMake2XUP::convertFromPro( s, v ), v ) )
	{
		mProjectFilePath = s;
		updateItem();
		setModified( false );
		checkChildrenProjects();
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
	return ProjectItem::filePath( s );
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
