#include "QMakeXUPItem.h"
#include "ProjectItemModel.h"
#include "XUPManager.h"
#include "QMake2XUP.h"

#include "pMonkeyStudio.h"

#include <QFileInfo>
#include <QApplication>
#include <QInputDialog>
#include <QDebug>

QMakeXUPItem::QMakeXUPItem( const QDomElement& e, const QString& s, bool b )
	: XUPItem()
{
	registerItem();
	setDomElement( e );
	loadProject( s );
	setModified( b );
}

QHash<QString, QStringList> QMakeXUPItem::variableSuffixes() const
{ return mVariableSuffixes; }

void QMakeXUPItem::registerVariableSuffixes( const QString& n, const QStringList& s )
{ mVariableSuffixes[n] = s; }

QIcon QMakeXUPItem::getIcon( const QString& o, const QString& d ) const
{
	return XUPItem::getIcon( o, d );
	QString s = QFile::exists( o ) ? o : QString( ":/qmakeitems/%1.png" ).arg( d );
	return QFile::exists( s ) ? QIcon( s ) : XUPItem::getIcon( o, d );
}

void QMakeXUPItem::registerItem()
{
	fix me by being static becasue now it's slow like the death
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
		<< "DEFINES"
		<< "OTHER_FILES";
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
		<< "SUBDIRS"
		<< "OTHER_FILES";
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
	mVariableLabels[ "OTHER_FILES" ] = tr( "Other Files" );
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
	mVariableIcons[ "OTHER_FILES" ] = getIcon( value( "icon" ), "other_files" );
	// register project suffixes
	mSuffixes[tr( "Qt Project" )] = QStringList( "*.pro" );
	mSuffixes[tr( "Qt Project Include" )] = QStringList( "*.pri" );
	// register variable suffixes
	// C++ filters
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
			yf << s;
	// IMAGES filters
	const QStringList pf = pMonkeyStudio::availableImageFormats();
	// PROJECT filters
	QStringList pjf;
	foreach ( QStringList l, mSuffixes.values() )
		pjf << l;
	// registering...
	mVariableSuffixes[ "HEADERS" ] = hf;
	mVariableSuffixes[ "SOURCES" ] = sf;
	mVariableSuffixes[ "OBJECTIVE_SOURCES" ] = QStringList( "*.m" ) << "*.mm";
	mVariableSuffixes[ "LEXSOURCES" ] = lf;
	mVariableSuffixes[ "YACCSOURCES" ] = yf;
	mVariableSuffixes[ "IMAGES" ] = pf;
	mVariableSuffixes[ "FORMS" ] = QStringList( "*.ui" );
	mVariableSuffixes[ "FORMS3" ] = QStringList( "*.ui" );
	mVariableSuffixes[ "TRANSLATIONS" ] = QStringList( "*.ts" );
	mVariableSuffixes[ "RESOURCES" ] = QStringList( "*.qrc" );
	mVariableSuffixes[ "DEF_FILE" ] = QStringList( "*.def" );
	mVariableSuffixes[ "RC_FILE" ] = QStringList( "*.rc" );
	mVariableSuffixes[ "RES_FILE" ] = QStringList( "*.res" );
}

QMakeXUPItem* QMakeXUPItem::clone( bool b ) const
{ return b ? new QMakeXUPItem( domElement(), projectFilePath(), modified() ) : new QMakeXUPItem; }

QString QMakeXUPItem::interpretedVariable( const QString& s, const XUPItem* it, const QString& d ) const
{
	return XUPItem::interpretedVariable( s, it, d );
	/*
		$$[QT_INSTALL_HEADERS] : read content from qt conf
		$${QT_INSTALL_HEADERS} or $$QT_INSTALL_HEADERS : read content from var
		$$(QT_INSTALL_HEADERS) : read from environment when qmake run
		$(QTDIR) : read from generated makefile
	*/
	QString dv = QString( s ).replace( '$', "" ).replace( '{', "" ).replace( '}', "" ).replace( '[', "" ).replace( ']', "" ).replace( '(', "" ).replace( ')', "" );
	// environment var
	if ( s.startsWith( "$$(" ) || dv == "PWD" )
		return dv != "PWD" ? qgetenv( qPrintable( dv ) ) : projectPath();
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

QString QMakeXUPItem::defaultInterpretedValue() const
{
	return XUPItem::defaultInterpretedValue();
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

void QMakeXUPItem::checkChildrenProjects()
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
						it->project()->appendRow( pi );
					else
						delete pi;
				}
			}
		}
		else if ( it->isType( "function" ) && it->defaultValue().startsWith( "include" ) )
		{
			// get filename
			fn = it->defaultInterpretedValue();
			fn.remove( 0, fn.indexOf( '(' ) +1 );
			fn.chop( fn.length() -fn.lastIndexOf( ')' ) );
			fn = it->filePath( fn.trimmed() );
			
			// open project include if it exists
			if ( QFile::exists( fn ) )
			{
				XUPItem* pi = clone( false );
				if ( pi->loadProject( fn ) )
					it->appendRow( pi );
				else
				{
					delete pi;
					qWarning( "QMakeXUPItem: error loading include file: %s", qPrintable( fn ) );
				}
			}
		}
	}
}

bool QMakeXUPItem::isProjectContainer() const
{ return interpretedVariable( "TEMPLATE", this ) == "subdirs"; }

bool QMakeXUPItem::loadProject( const QString& s, const QString& v )
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

bool QMakeXUPItem::saveProject( const QString& s, const QString& v )
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

void QMakeXUPItem::addFiles( const QStringList& files, XUPItem* scope, const QString& op )
{
	// abort if no files
	if ( files.isEmpty() )
		return;
	
	// set scope
	if ( !scope )
		scope = this;
	
	// remember if it's a project container or not
	bool container = isProjectContainer();
	
	foreach ( QString file, files )
	{
		QStringList vnl;
		// get variables that can handle this file
		if ( container )
		{
			// check project extension
			foreach ( QString vn, mSuffixes.keys() )
				if ( QDir::match( mSuffixes[vn], file ) )
					vnl << "SUBDIRS";
			
			// if no variable it mean not proejct file, process next file
			if ( vnl.isEmpty() )
				continue;
		}
		else
		{
			// variables names that can handle this suffix
			foreach ( QString vn, mVariableSuffixes.keys() )
			if ( QDir::match( mVariableSuffixes[vn], file ) )
				vnl << vn;
		}
		
		// request variable name to user if more than 1
		QString vn;
		if ( vnl.count() > 1 )
		{
			bool ok;
			vn = QInputDialog::getItem( QApplication::activeWindow(), tr( "Choose variable..." ), tr( "More than one variable can handle this file,\nplease select the variable you want to use for this file :\n%1" ).arg( QFileInfo( file ).fileName() ), vnl, 0, false, &ok );
			if ( !ok )
				vn.clear();
		}
		else
			vn = vnl.value( 0 );
		if ( vn.isEmpty() )
			vn = "OTHER_FILES";
		
		// get variable item
		XUPItem* vit = 0;
		foreach ( XUPItem* cit, scope->children( false, true ) )
		{
			if ( cit->isType( "variable" ) && cit->defaultValue() == vn && cit->value( "operator" ) == op )
			{
				vit = cit;
				break;
			}
		}
		
		// check variable exists or not
		bool exists = vit;
	
		// create variable if needed
		if ( !exists )
		{
			vit = clone( false );
			vit->setDomElement( mDomElement.ownerDocument().createElement( "variable" ) );
			scope->domElement().appendChild( vit->domElement() );
			vit->setValue( valueName(), vn );
			vit->setValue( "operator", op );
		}
	
		// set variable multiline
		vit->setValue( "multiline", "true" );
	
		// get all files
		QStringList existingFiles = vit->files();
	
		// add file
		QString fp = filePath( file );
		if ( !existingFiles.contains( fp ) )
		{
			XUPItem* it = clone( false );
			it->setDomElement( mDomElement.ownerDocument().createElement( "value" ) );
			vit->domElement().appendChild( it->domElement() );
			it->setValue( it->valueName(), relativeFilePath( fp ) );
			vit->appendRow( it );
		}
	
		// append item to tree if needed
		if ( !exists )
			scope->appendRow( vit );
		
		// open sub project if needed
		if ( vn == "SUBDIRS" && container )
		{
			XUPItem* pi = clone( false );
			if ( pi->loadProject( file ) )
				project()->appendRow( pi );
			else
				delete pi;
		}
	}
}

void QMakeXUPItem::removeFiles( const QStringList& files, XUPItem* scope, const QString& op )
{
	// abort if no files
	if ( files.isEmpty() )
		return;
	
	// set scope
	if ( !scope )
		scope = this;
	
	// check variables
	foreach ( XUPItem* vit, scope->children( false, true ) )
	{
		if ( vit->isType( "variable" ) && vit->value( "operator" ) == op )
		{
			// check each child
			foreach ( XUPItem* cit, vit->children( false, false ) )
				if ( cit->isType( "value" ) && files.contains( cit->filePath() ) )
					cit->remove();
			
			// remove variable item if emptyline
			if ( !vit->hasChildren() )
				vit->remove();
		}
	}
}

QString QMakeXUPItem::filePath( const QString& s )
{
	return XUPItem::filePath( s );
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

QStringList QMakeXUPItem::splitFiles( const QString& s ) const
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
