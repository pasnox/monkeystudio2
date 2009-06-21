/****************************************************************************
**
** 		Created using Monkey Studio v1.8.1.0
** Authors    : Filipe AZEVEDO aka Nox P@sNox <pasnox@gmail.com>
** Project   : Monkey Studio Project Plugins
** FileName  : QMakeItem.cpp
** Date      : 2008-01-14T00:54:11
** License   : GPL
** Comment   : This header has been automatically generated, if you are the original author, or co-author, fill free to replace/append with your informations.
** Home Page : http://www.monkeystudio.org
**
    Copyright (C) 2005 - 2008  Filipe AZEVEDO & The Monkey Studio Team

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
**
****************************************************************************/
#include "QMakeItem.h"
#include "UISettingsQMake.h"
#include "QMakeParser.h"
#include "UIQMakeProjectSettings.h"
#include "QMake.h"

#include <monkey.h>

using namespace pMonkeyStudio;

ProjectItem* variable( ProjectItem* s, const QString& v, const QString& o )
{
	Q_ASSERT( s );
	QStringList l = UISettingsQMake::readPathFiles();
	foreach ( ProjectItem* it, s->children( false, true ) )
	{
		if ( it->getValue() == v && it->getOperator() == o )
		{
			it->setMultiLine( l.contains( v ) && !v.contains( "path", Qt::CaseInsensitive ) );
			return it;
		}
	}
	ProjectItem* it = new QMakeItem( ProjectItem::VariableType, s );
	it->setValue( v );
	it->setOperator( o );
	it->setMultiLine( l.contains( v ) && !v.contains( "path", Qt::CaseInsensitive ) );
	return it;
}

QString evaluate( const QString s, const ProjectItem* p )
{
	Q_ASSERT( p );
	QString v;
	foreach ( ProjectItem* it, p->match( ProjectItem::VariableType, s ) )
	{
		const QString o = it->getOperator();
		foreach ( ProjectItem* cit, it->children() )
		{
			if ( o == "=" )
				v = cit->getValue();
			else if ( o == "+=" )
				v.append( cit->getValue().prepend( " " ) );
			else if ( o == "-=" )
			{
				foreach ( const QString s, cit->getValue().split( " " ) )
					v.remove( s );
			}
			else if ( o == "*=" )
			{
				QStringList l = v.split( " " );
				foreach ( QString s, cit->getValue().split( " " ) )
					if ( !l.contains( s ) )
						v.append( s.prepend( " " ) );
			}
			else if ( o == "~=" )
			{
				qWarning( "QString evaluate( const QString s, ProjectItem* p ): Need Implementation" );
				/*
				QRegExp rx( 
				QStringList l = v.split( " " );
				foreach ( QString s, cit->getValue().split( " " ) )
					if ( !l.contains( s ) )
						v.append( s.prepend( " " ) );
					
					DEFINES ~= s/QT_[DT].+/QT
				*/
			}
		}
	}
	// return value
	return v.trimmed();
}

QMakeItem::QMakeItem( ProjectItem::NodeType t, ProjectItem* i )
{
	init();
	setType( t );
	if ( i )
		i->appendRow( this );
}

void QMakeItem::setType( ProjectItem::NodeType t )
{
	ProjectItem::setType( t );
	switch( t )
	{
		case ProjectItem::EmptyType:
			setIcon( QIcon( ":/icons/icons/line.png" ) );
			break;
		case ProjectItem::FolderType:
			setIcon( QIcon( ":/icons/icons/folder.png" ) );
			break;
		case ProjectItem::CommentType:
			setIcon( QIcon( ":/icons/icons/comment.png" ) );
			break;
		case ProjectItem::NestedScopeType:
			setIcon( QIcon( ":/icons/icons/scope.png" ) );
			break;
		case ProjectItem::ScopeType:
			setIcon( QIcon( ":/icons/icons/scope.png" ) );
			break;
		case ProjectItem::ScopeEndType:
			setIcon( QIcon( ":/icons/icons/scope_end.png" ) );
			break;
		case ProjectItem::VariableType:
			setIcon( QIcon( ":/icons/icons/variable.png" ) );
			break;
		case ProjectItem::ValueType:
			setIcon( QIcon( ":/icons/icons/value.png" ) );
			break;
		case ProjectItem::FunctionType:
			setIcon( QIcon( ":/icons/icons/function.png" ) );
			break;
		case ProjectItem::IncludeType:
			setIcon( QIcon( ":/icons/icons/include.png" ) );
			break;
		case ProjectItem::ProjectType:
			setIcon( QIcon( ":/icons/icons/project.png" ) );
			break;
		default:
			setIcon( QIcon() );
			break;
	}
}

void QMakeItem::setValue( const QString& s )
{
	// set data
	ProjectItem::setValue( s );
	// set text
	setText( s );
	// variables name to changes
	QStringList l1 = UISettingsQMake::readFilters();
	QStringList l2 = UISettingsQMake::readFiltersToolTips();
	QStringList l3 = UISettingsQMake::readPathFiles();
	// update text
	switch( getType() )
	{
		case ProjectItem::VariableType:
			if ( l1.contains( getValue(), Qt::CaseInsensitive ) )
				setText( l2.value( l1.indexOf( getValue(), Qt::CaseInsensitive ) ) );
			break;
		case ProjectItem::ValueType:
		{
			// get parent value
			QString v = parent() ? parent()->getValue() : QString::null;
			// if file/path base variable
			if ( l3.contains( v, Qt::CaseInsensitive ) )
			{
				// if special case subdirs
				if ( v.toLower() == "subdirs" )
				{
					// get path to check in for pro
					v = canonicalFilePath( s );
					// get last part of path
					QString c = s.section( '/', -1 ).toLower();
					// get all pro file
					foreach ( QFileInfo f, getFiles( QDir( v ), "*.pro", false ) )
					{
						// check that value = filename
						if ( f.baseName().toLower() == c )
						{
							// set full file path
							setFilePath( v.append( "/" ).append( f.fileName() ) );
							break;
						}
					}
				}
				// set full file path
				else
					setFilePath( s );
			}
			break;
		}
		case ProjectItem::ProjectType:
			setFilePath( s );
			break;
		case ProjectItem::ScopeType:
		case ProjectItem::NestedScopeType:
			if ( s.toLower() == "win32" )
				setIcon( QIcon( ":/icons/icons/windows.png" ) );
			else if ( s.toLower() == "unix" )
				setIcon( QIcon( ":/icons/icons/unix.png" ) );
			else if ( s.toLower() == "mac" )
				setIcon( QIcon( ":/icons/icons/mac.png" ) );
			break;
		default:
			break;
	}
}

void QMakeItem::setFilePath( const QString& s )
{
	// set data
	ProjectItem::setFilePath( canonicalFilePath( s ) );
	// set text
	switch( getType() )
	{
		case ProjectItem::ValueType:
			if ( parent() && parent()->getValue().toLower() != "subdirs" )
				setText( fileName( s ) );
			setToolTip( getFilePath() );
			break;
		case ProjectItem::ProjectType:
			setText( completeBaseName( s ) );
			setToolTip( getFilePath() );
			break;
		default:
			break;
	}
}

QString QMakeItem::getIndent() const
{
	if ( !parent() || parent()->isProject() || parent()->isNested() || ( isValue() && isFirst() ) || ( isScope( false ) && getValue().toLower() == "else" ) )
		return QString();
	else if ( isScopeEnd() )
	{
		if ( !( parent()->isScope( false ) && parent()->getValue().toLower() == "else" ) )
			return parent()->getIndent();
		else
		{
			ProjectItem* p = parent()->parent()->child( parent()->row() -1 );
			return p->child( p->rowCount() -1 )->getIndent();
		}
	}
	return QString().fill( '\t', parentCount() -1 );
}

bool QMakeItem::isFirst() const
{ return row() == 0; }

bool QMakeItem::isLast() const
{
	if ( model() )
		return model()->rowCount( index().parent() ) -1 == row();
	else if ( parent() )
		return parent()->rowCount() -1 == row();
	Q_ASSERT( 0 );
	return false;
}

bool QMakeItem::isProjectsContainer() const
{ return isProject() ? evaluate( "TEMPLATE", const_cast<QMakeItem*>( this ) ).toLower() == "subdirs" : false; }

void QMakeItem::insertRow( int r, ProjectItem* it )
{
	// check scope & function item for not be able to append items after scope end
	if ( isScope() && it &&  !it->isScopeEnd() )
		r--;
	// if there is subproject, we need to create items before subproject
	if ( hasChildren() )
	{
		ProjectItem* p = project();
		ProjectItem* i = 0;
		while ( ( i = child( r -1 ) ) && i->project() != p )
			r--;
	}
	// default insert
	QStandardItem::insertRow( r, it );
	// if project get parent plugin
	if ( it->isProject() )
		it->setPlugin( project()->plugin() );
}

bool QMakeItem::open()
{
	if ( isProject() )
	{
		if ( !QFile::exists( getValue() ) )
			return false;
		// populate datas
		QMakeParser p( getValue(), this );
		// set proejct writable
		setReadOnly( false );
		// set project unmodified
		setModified( false );
		// refresh project
		refresh();
		// same for childproject
		foreach ( ProjectItem* p, childrenProjects() )
		{
			p->setReadOnly( false );
			p->setModified( false );
			p->refresh();
		}
		// return state
		return p.isOpen();
	}
	return false;
}

bool QMakeItem::addProject( const QString& f, ProjectItem* s, const QString& o )
{
	if ( !isProjectsContainer() || getReadOnly() )
		return false;
	// check if already present value
	ProjectItem* v = variable( s, "SUBDIRS", o );
	foreach ( ProjectItem* cit, v->children( false ) )
		if ( isSameFile( cit->getFilePath(), f ) )
			return true;
	// add value to subdirs
	ProjectItem* it = new QMakeItem( ProjectItem::ValueType, v );
	it->setValue( relativePath( f ) );
	// if project already open as a subproject, don't reopen it
	foreach ( ProjectItem* it, childrenProjects( false ) )
		if ( isSameFile( it->getFilePath(), f ) )
			return true;
	// create project item
	if ( ProjectItem* pit = plugin()->getProjectItem( f ) )
	{
		if ( pit->open() )
		{
			// add project
			appendRow( pit );
			pit->setModified( false );
			return true;
		}
		delete pit;
	}
	// not added
	return false;
}

void QMakeItem::editSettings()
{ if ( isProject() ) UIQMakeProjectSettings::instance( this )->exec(); }

void QMakeItem::save( bool b )
{
	// only project item can call this and must be not read only and modified
	if ( !isProject() || getReadOnly() || !getModified() )
		return;
	// ask user to save if needed
	if ( !b || ( b && question( tr( "Save Project..." ), tr( "The project [%1] has been modified, save it ?" ).arg( name() ) ) ) )
		writeProject();
}

void QMakeItem::addExistingFiles( const QStringList& l, ProjectItem* s, const QString& o )
{
	if ( !isProject() )
		return;
	// C++ filters
	const QStringList cf = availableLanguagesSuffixes().value( "C++" );
	// YACC filters
	QStringList yf;
	foreach ( QString s, cf )
		if ( s.startsWith( "*.c", Qt::CaseInsensitive ) && !yf.contains( s.replace( "c", "y", Qt::CaseInsensitive ) ) )
			yf << s;
	// LEX filters
	QStringList lf;
	foreach ( QString s, cf )
		if ( s.startsWith( "*.c", Qt::CaseInsensitive ) && !lf.contains( s.replace( "c", "l", Qt::CaseInsensitive ) ) )
			yf << s;
	// IMAGES filters
	const QStringList pf = availableImageFormats();
	// PROJECT filters
	const QStringList pjf = plugin()->suffixes().values().value( 0 );
	// iterate each file
	foreach ( const QString& f, l )
	{
		// project
		if ( QDir::match( pjf, f ) )
		{
			if ( !isProjectsContainer() )
				warning( tr( "Add Existing Files..." ), tr( "This project is not a projects container, cancel adding project:\n%1" ).arg( f ) );
			else if( !addProject( f, s, o ) )
				warning( tr( "Add Existing Files..." ), tr( "A selected file is a project that i can't open:\n%1" ).arg( f ) );
			continue;
		}
		else if ( isProjectsContainer() )
			continue;
		// files
		QString vn = "UNKNOW_FILES";
		if ( QDir::match( cf, f ) )
		{
			if ( QFileInfo( f ).suffix().startsWith( "h", Qt::CaseInsensitive ) )
				vn = "HEADERS";
			else
				vn = "SOURCES";
		}
		else if ( QDir::match( lf, f ) )
			vn = "LEXSOURCES";
		else if ( QDir::match( yf, f ) )
			vn = "YACCSOURCES";
		else if ( QDir::match( pf, f ) )
			vn = "IMAGES";
		else if ( QDir::match( "*.ui", f ) )
		{
			vn = "FORMS";
			if ( question( tr( "FORMS3 Files..." ), tr( "Do i need to add this form to FORMS3 variable ?" ) ) )
				vn = "FORMS3";
		}
		else if ( QDir::match( "*.ts", f ) )
			  vn = "TRANSLATIONS";
		else if ( QDir::match( "*.qrc", f ) )
			  vn = "RESOURCES";
		else if ( QDir::match( "*.def", f ) )
			  vn = "DEF_FILE";
		else if ( QDir::match( "*.rc", f ) )
			vn = "RC_FILE";
		else if ( QDir::match( "*.res", f ) )
			vn = "RES_FILE";
		// create value for variable
		ProjectItem* v = variable( s, vn, o );
		bool b = false;
		foreach ( ProjectItem* cit, v->children( false ) )
		{
			if ( isSameFile( cit->getFilePath(), f ) )
			{
				b = true;
				break;
			}
		}
		if ( b )
			continue;
		// create item value
		ProjectItem* it = new QMakeItem( ProjectItem::ValueType, v );
		it->setValue( vn != "UNKNOW_FILES" ? relativeFilePath( f ) : f );
	}
}

void QMakeItem::addCommand( const pCommand& c, const QString& s )
{
	// check validity
	if ( !c.isValid() )
		return;
	// get menu bar pointer
	pMenuBar* mb = MonkeyCore::menuBar();
	// create action
	QAction* a = mb->action( QString( "%1/%2" ).arg( s, c.text() ) , c.text() );
	a->setData( PLUGIN_NAME );
	a->setStatusTip( c.text() );
	connect( a, SIGNAL( triggered() ), this, SLOT( commandTriggered() ) );
	// add command to command list
	mCommands << c;
}

void QMakeItem::installCommands()
{
	// builder/compiler
	if ( isProject() && builder() )
	{
		// get current qt version
		QtVersion qv = UISettingsQMake::defaultVersion();
		// temp command
		pCommand c;
		// clear commands
		mCommands.clear();
		// get builder commnd
		pCommand bcc = builder()->buildCommand();
		if ( compiler() )
			bcc.addParsers( compiler()->compileCommand().parsers() );
		const pCommand bc = bcc;
		// evaluate some var
		QString target = evaluate( "TARGET", this );
		QString destdir = evaluate( "DESTDIR", this );
		if ( destdir.isEmpty() )
			destdir = evaluate( "DLLDESTDIR", this );
#ifdef Q_OS_WIN
		if ( !destdir.left( 2 ).endsWith( ':' ) )
			destdir.prepend( "$cpp$/" );
#else
		if ( !destdir.startsWith( '/' ) )
			destdir.prepend( "$cpp$/" );
#endif
		if ( destdir.endsWith( '/' ) )
			destdir.chop( 1 );
		// build debug
		c = bc;
		c.setText( tr( "Build Debug" ) );
		c.setArguments( "debug" );
		addCommand( c, "mBuilder/mBuild" );
		// build release
		c = bc;
		c.setText( tr( "Build Release" ) );
		c.setArguments( "release" );
		addCommand( c, "mBuilder/mBuild" );
		// build all
		c = bc;
		c.setText( tr( "Build All" ) );
		c.setArguments( "all" );
		addCommand( c, "mBuilder/mBuild" );
		// simple build call
		addCommand( bc, "mBuilder/mBuild" );
		// clean
		c = bc;
		c.setText( tr( "Clean" ) );
		c.setArguments( "clean" );
		addCommand( c, "mBuilder/mClean" );
		// distclean
		c = bc;
		c.setText( tr( "Distclean" ) );
		c.setArguments( "distclean" );
		addCommand( c, "mBuilder/mClean" );
		// qmake command
		c = pCommand();
		c.setText( tr( "QMake" ) );
		c.setCommand( qv.QMake );
#ifdef Q_OS_MAC
		c.setArguments( "-spec macx-g++" );
#endif
		c.setWorkingDirectory( "$cpp$" );
		addCommand( c, "mBuilder" );
		// lupdate command
		c = pCommand();
		c.setText( tr( "lupdate" ) );
		c.setCommand( qv.lupdate );
		c.setArguments( "$cp$" );
		c.setWorkingDirectory( "$cpp$" );
		addCommand( c, "mBuilder" );
		// lrelease command
		c = pCommand();
		c.setText( tr( "lrelease" ) );
		c.setCommand( qv.lrelease );
		c.setArguments( "$cp$" );
		c.setWorkingDirectory( "$cpp$" );
		addCommand( c, "mBuilder" );
		// execute debug
		c = bc;
		c.setText( tr( "Execute Debug" ) );
		c.setCommand( target.isEmpty() ? name() : target );
		c.setArguments( QString::null );
		c.setWorkingDirectory( destdir != "$cpp$" ? destdir : destdir +"/debug" );
		addCommand( c, "mBuilder/mExecute" );
		// execute release
		c = bc;
		c.setText( tr( "Execute Release" ) );
		c.setCommand( target.isEmpty() ? name() : target );
		c.setArguments( QString::null );
		c.setWorkingDirectory( destdir != "$cpp$" ? destdir : destdir +"/release" );
		addCommand( c, "mBuilder/mExecute" );
		// rebuild debug
		c = bc;
		c.setText( tr( "Rebuild Debug" ) );
		c.setCommand( QStringList( QStringList() << tr( "QMake" ) << tr( "Distclean" ) << tr( "QMake" ) << tr( "Build Debug" ) ).join( ";" ) );
		c.setArguments( QString::null );
		addCommand( c, "mBuilder/mRebuild" );
		// rebuild release
		c = bc;
		c.setText( tr( "Rebuild Release" ) );
		c.setCommand( QStringList( QStringList() << tr( "QMake" ) << tr( "Distclean" ) << tr( "QMake" ) << tr( "Build Release" ) ).join( ";" ) );
		c.setArguments( QString::null );
		addCommand( c, "mBuilder/mRebuild" );
		// rebuild all
		c = bc;
		c.setText( tr( "Rebuild All" ) );
		c.setCommand( QStringList( QStringList() << tr( "QMake" ) << tr( "Distclean" ) << tr( "QMake" ) << tr( "Build All" ) ).join( ";" ) );
		c.setArguments( QString::null );
		addCommand( c, "mBuilder/mRebuild" );
		// simple rebuild call
		c = bc;
		c.setText( tr( "Rebuild" ) );
		c.setCommand( QStringList( QStringList() << tr( "QMake" ) << tr( "Distclean" ) << tr( "QMake" ) << tr( "Build" ) ).join( ";" ) );
		c.setArguments( QString::null );
		addCommand( c, "mBuilder/mRebuild" );
	}
}

void recursiveRemoveCommands( QMenu* m )
{
	Q_ASSERT( m );
	foreach ( QAction* a, m->actions() )
	{
		if ( a->menu() )
			recursiveRemoveCommands( a->menu() );
		else if ( !a->isSeparator() && a->data().toString() == PLUGIN_NAME )
			delete a;
	}
}

void QMakeItem::uninstallCommands()
{ recursiveRemoveCommands( MonkeyCore::menuBar()->menu( "mBuilder" ) ); }

void QMakeItem::setValues( ProjectItem* it, const QString& v, const QString& o, const QStringList& l )
{
	// need item
	Q_ASSERT( it );
	// search variable item
	ProjectItem* vi = 0;
	foreach ( ProjectItem* cit, it->children( false, true ) )
	{
		if ( cit->getValue() == v && cit->getOperator() == o )
		{
			vi = cit;
			break;
		}
	}
	// if no variable found, and values is empty return
	if ( !vi && l.isEmpty() )
		return;
	// if values is empty delete item
	else if ( vi && l.isEmpty() )
		vi->remove();
	else
	{
		// create variable if needed
		if ( !vi )
		{
			vi = new QMakeItem( ProjectItem::VariableType, it );
			vi->setValue( v );
			vi->setOperator( o );
		}
		// update / create items
		for ( int i = 0; i < l.count(); i++ )
		{
			const QString s = l.at( i );
			ProjectItem* cvi = vi->child( i );
			// update no longer needed value
			if ( cvi && !l.contains( cvi->getValue() ) )
				cvi->setValue( s );
			// swap existing value if needed to contrains ordered values
			else if ( cvi && l.contains( cvi->getValue() ) )
			{
				if ( s != cvi->getValue() )
				{
					foreach ( ProjectItem* cit, vi->children() )
					{
						if ( cit->getValue() == s )
						{
							bool b = model()->blockSignals( true );
							vi->swapRow( cit->row(), i );
							model()->blockSignals( b );
							break;
						}
					}
				}
			}
			// create require item value
			else if ( !cvi || ( cvi && cvi->getValue() != s ) )
			{
				cvi = new QMakeItem( ProjectItem::ValueType, vi );
				cvi->setValue( s );
			}
		}
		// remove no longer need items
		while ( ProjectItem* cvi = vi->child( l.count() ) )
			cvi->remove();
	}
}

void QMakeItem::addValue( ProjectItem* it, const QString& v, const QString& o, const QString& vl )
{
	// need item
	Q_ASSERT( it );
	// search variable item
	ProjectItem* vi = 0;
	foreach ( ProjectItem* cit, it->children( false, true ) )
	{
		if ( cit->getValue() == v && cit->getOperator() == o )
		{
			vi = cit;
			break;
		}
	}
	// if no variable found, and values is empty return
	if ( !vi && vl.isEmpty() )
		return;
	// if values is empty delete item
	else if ( vi && vl.isEmpty() )
		vi->remove();
	else
	{
		// create variable if needed
		if ( !vi )
		{
			vi = new QMakeItem( ProjectItem::VariableType, it );
			vi->setValue( v );
			vi->setOperator( o );
		}
		// check values
		foreach ( ProjectItem* cit, vi->children() )
			if ( ( vi->getMultiLine() && cit->getValue() == vl ) || ( !vi->getMultiLine() && cit->getValue().contains( QRegExp( QString( "(^|\\b)%1(\\b|$)" ).arg( QRegExp::escape( vl ) ) ) ) ) )
				return;
		// if there, value don t exists, create it
		ProjectItem* cvi = new QMakeItem( ProjectItem::ValueType, vi );
		cvi->setValue( vl );
	}
}

ProjectItemList QMakeItem::getValues( ProjectItem* s, const QString& v, const QString& o ) const
{
	Q_ASSERT( s );
	ProjectItemList l;
	foreach ( ProjectItem* it, s->children( false, true ) )
		if ( it->isVariable() && it->getValue() == v && it->getOperator() == o )
			foreach ( ProjectItem* cit, it->children() )
				l << cit;
	return l;
}

void QMakeItem::redoLayout( ProjectItem* it ) //
{
	// get correct item
	it = it ? it : project();
	
	// variables name to changes
	QStringList l1 = UISettingsQMake::readFilters();
	
	// layout it child
	for ( int i = 0; i < it->rowCount(); i++ )
	{
		// get child
		ProjectItem* c = it->child( i );
		
		// chld value
		int j = l1.indexOf( c->getValue(), Qt::CaseInsensitive );
		
		// change it s filter role if needed
		if ( c->isVariable() )
		{
			// set child filter
			c->setFilteredView( j );
			
			// if it is scope/nestedscope change it s filter too
			if ( it->isScope() )
				it->setFilteredView( j );
		}
		
		// set original view
		c->setOriginalView( i );
		
		// do child layout if needed
		if ( c->rowCount() )
			redoLayout( c );
	}
}

bool QMakeItem::writeProject()
{
	// only project item can call this
	if ( !isProject() )
		return false;
	
	// clear buffer
	mBuffer.clear();

	// write items
	foreach ( ProjectItem* it, children( false, true) )
		writeItem( it );
	
	// write file
	QFile f( getValue() );
	if ( f.open( QFile::WriteOnly ) && f.resize( 0 ) && f.write( mBuffer ) != -1 )
	{
		// set project unmodified
		setModified( false );
		
		// return success
		return true;
	}
	
	// default return value
	return false;
}

void QMakeItem::writeItem( ProjectItem* it )
{
	// cancel if no item
	if ( !it )
		return;
	
	// write to buffer
	switch ( it->getType() )
	{
		case ProjectItem::EmptyType:
		{
			mBuffer.append( it->getIndent() ).append( getEol() );
			break;
		}
		case ProjectItem::FolderType:
		{
			break;
		}
		case ProjectItem::CommentType:
		{
			mBuffer.append( it->getIndent() ).append( it->getValue() ).append( getEol() );
			break;
		}
		case ProjectItem::NestedScopeType:
		{
			mBuffer.append( it->getIndent() ).append( it->getValue() ).append( it->getOperator() );
			break;
		}
		case ProjectItem::ScopeType:
		{
			mBuffer.append( it->getIndent() ).append( it->getValue() ).append( " {" ).append( it->getComment().isEmpty() ? "" : it->getComment().prepend( " " ) ).append( getEol() );
			break;
		}
		case ProjectItem::ScopeEndType:
		{
			if ( it->parent()->isScope( false ) )
			{
				// parent sibling
				ProjectItem* p = it->parent()->parent()->child( it->parent()->row() +1, 0 );
				if ( p && ( !p->isScope() || p->getValue().toLower() != "else" ) )
					p = 0;
				mBuffer.append( it->getIndent() ).append( "}" ).append( it->getComment().isEmpty() ? "" : it->getComment().prepend( " " ) ).append( p ? " " : getEol() );
			}
			break;
		}
		case ProjectItem::VariableType:
		{
			mBuffer.append( it->getIndent() ).append( it->getValue() ).append( "\t" ).append( it->getOperator() ).append( " " );
			break;
		}
		case ProjectItem::ValueType:
		{
			QString s = it->getValue();
			if ( UISettingsQMake::readPathFiles().contains( it->parent()->getValue() ) )
				s = quotedString( s );
			if ( it->parent()->getMultiLine() )
				mBuffer.append( it->getIndent() ).append( s ).append( !it->isLast() ? " \\" : "" ).append( it->getComment().isEmpty() ? "" : it->getComment().prepend( " " ) ).append( getEol() );
			else
				mBuffer.append( s ).append( it->getComment().isEmpty() ? "" : it->getComment().prepend( " " ) ).append( it->isLast() ? getEol() : " " );
			break;
		}
		case ProjectItem::FunctionType:
		{
			mBuffer.append( it->getIndent() ).append( it->getValue() ).append( it->getComment().isEmpty() ? "" : it->getComment().prepend( " " ) ).append( getEol() );
			break;
		}
		case ProjectItem::IncludeType:
		{
			mBuffer.append( it->getIndent() ).append( it->getValue() ).append( it->getComment().isEmpty() ? "" : it->getComment().prepend( " " ) ).append( getEol() );
			break;
		}
		case ProjectItem::ProjectType:
		{
			break;
		}
		default:
			break;
	}
	
	// write children
	foreach ( ProjectItem* cit, it->children( false, true ) )
		writeItem( cit );
}

void QMakeItem::commandTriggered()
{
	pConsoleManager* cm = MonkeyCore::consoleManager();
	pCommandList l = mCommands;
	if ( QAction* a = qobject_cast<QAction*>( sender() ) )
	{
		// if build/rebuild action
		if ( a->text().contains( "build", Qt::CaseInsensitive ) )
		{
			ProjectItemList l;
			// if auto increment build version
			l = getValues( this, "APP_AUTO_INCREMENT", "=" );
			if ( l.count() && l.at( 0 )->getValue() == "1" )
			{
				// update version
				l = getValues( this, "VERSION", "=" );
				if ( l.count() )
				{
					QStringList v = l.at( 0 )->getValue().split( "." );
					while ( v.count() < 4 )
						v.append( "0" );
					int b = v.at( 3 ).toInt();
					b++;
					v[3] = QString::number( b );
					setValues( this, "VERSION", "=", QStringList( v.join( "." ) ) );
					// update define
					addValue( this, "DEFINES", "+=", "\"PROGRAM_VERSION=\\\"\\\\\\\"$${VERSION}\\\\\\\"\\\"\"" );
				}
			}
			// save projects if user request
			saveAll();
		}
		else if ( a->text().contains( "execute", Qt::CaseInsensitive ) )
		{
			pCommand c = cm->processCommand( cm->getCommand( l, a->statusTip() ) );
			QString s = QString( "%1/%2" ).arg( c.workingDirectory() ).arg( c.command() );
			if ( !QFile::exists( s ) )
			{
				// try reading already saved binary
				s = getStringValues( this, a->text().replace( ' ', '_' ).toUpper(), "=" ).value( 0 );
				if ( !s.isEmpty() )
					s = canonicalFilePath( s );
				// if not exists ask user to select one
				if ( !QFile::exists( s ) && question( a->text().append( "..." ), tr( "Can't find your executable file, do you want to choose the file ?" ).arg( s ) ) )
					s = getOpenFileName( a->text().append( "..." ), c.workingDirectory() );
				// if file exists execut it
				if ( QFile::exists( s ) )
				{
					QFileInfo fi( s );
					QString f = fi.fileName().prepend( "./" );
					QString p = fi.absolutePath();
					if ( p.endsWith( '/' ) )
						p.chop( 1 );
					// correct command
					c.setCommand( cm->quotedString( cm->nativeSeparators( s ) ) );
					c.setWorkingDirectory( cm->nativeSeparators( p ) );
					// add command to console manager
					cm->addCommand( c );
					// write in project
					setValues( this, a->text().replace( ' ', '_' ).toUpper(), "=", QStringList( relativeFilePath( s ) ) );
				}
			}
			// return
			return;
		}
		
		// add commands to console
		cm->addCommands( cm->recursiveCommandList( l, cm->getCommand( l, a->statusTip() ) ) );
	}
}

void QMakeItem::setBuilder( BuilderPlugin* b )
{
	if ( isProject() )
		setValues( this, "BUILDER", "=", b ? QStringList( b->infos().Name ) : QStringList() );
}

BuilderPlugin* QMakeItem::builder() const
{
	if ( isProject() )
	{
		QString s = evaluate( "BUILDER", this );
		if ( s.isEmpty() && parentProject() )
			if ( BuilderPlugin* b = parentProject()->builder() )
				s = b->infos().Name;
		if ( s.isEmpty() )
			s = "GNUMake";
		return MonkeyCore::pluginsManager()->plugin<BuilderPlugin*>( PluginsManager::stAll, s );
	}
	return 0;
}

void QMakeItem::setCompiler( CompilerPlugin* c )
{
	if ( isProject() )
		setValues( this, "COMPILER", "=", c ? QStringList( c->infos().Name ) : QStringList() );
}

CompilerPlugin* QMakeItem::compiler() const
{
	if ( isProject() )
	{
		QString s = evaluate( "COMPILER", this );
		if ( s.isEmpty() && parentProject() )
			if ( CompilerPlugin* c = parentProject()->compiler() )
				s = c->infos().Name;
		if ( s.isEmpty() )
			s = "G++";
		return MonkeyCore::pluginsManager()->plugin<CompilerPlugin*>( PluginsManager::stAll, s );
	}
	return 0;
}

void QMakeItem::setDebugger( DebuggerPlugin* d )
{
	if ( isProject() )
		setValues( this, "DEBUGGER", "=", d ? QStringList( d->infos().Name ) : QStringList() );
}

DebuggerPlugin* QMakeItem::debugger() const
{
	if ( isProject() )
	{
		QString s = evaluate( "DEBUGGER", this );
		if ( s.isEmpty() && parentProject() )
			if ( DebuggerPlugin* d = parentProject()->debugger() )
				s = d->infos().Name;
		if ( s.isEmpty() )
			s = "GNUDebugger";
		return MonkeyCore::pluginsManager()->plugin<DebuggerPlugin*>( PluginsManager::stAll, s );
	}
	return 0;
}

void QMakeItem::setInterpreter( InterpreterPlugin* i )
{
	if ( isProject() )
		setValues( this, "INTERPRETER", "=", i ? QStringList( i->infos().Name ) : QStringList() );
}

InterpreterPlugin* QMakeItem::interpreter() const
{
	if ( isProject() )
	{
		QString s = evaluate( "INTERPRETER", this );
		if ( s.isEmpty() && parentProject() )
			if ( InterpreterPlugin* i = parentProject()->interpreter() )
				s = i->infos().Name;
		return MonkeyCore::pluginsManager()->plugin<InterpreterPlugin*>( PluginsManager::stAll, s );
	}
	return 0;
}
