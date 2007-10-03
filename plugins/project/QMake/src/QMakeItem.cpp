/****************************************************************************
**
** 		Created using Monkey Studio v1.7.0
** Author    : Nox P@sNox aka Azevedo Filipe <pasnox@gmail.com>
** Project   : QMakeItem
** FileName  : QMakeItem.cpp
** Date      : 2007-09-04T15:07:31
** License   : GPL
** Comment   : Your comment here
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/
#include "QMakeItem.h"
#include "ProjectsModel.h"
#include "UISettingsQMake.h"
#include "pMonkeyStudio.h"
#include "QMakeParser.h"
#include "UIQMakeProjectSettings.h"
#include "QMake.h"

using namespace pMonkeyStudio;

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
					foreach ( QFileInfo f, getFiles( QDir( v ), "pro", false ) )
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

QString QMakeItem::getIndent( bool b ) const
{
	//if ( !b )
	{
		if ( !parent() || parent()->isNested() || ( isValue() && isFirst() ) || ( isScope() && getValue().toLower() == "else" ) )
			return QString();
		else if ( isScopeEnd() )
			return parent()->getIndent( true );
	}
	//if ( isScope() && getValue().toLower() == "else" )
		//return QString().fill( '\t', parentCount() -1 -scope().count( "else", Qt::CaseInsensitive ) );
	return QString().fill( '\t', parentCount() -1 );
}

QString QMakeItem::getEol() const
{
	switch ( eolMode() )
	{
		case QsciScintilla::EolWindows:
			return QString( "\r\n" );
			break;
		case QsciScintilla::EolUnix:
			return QString( "\r" );
			break;
		case QsciScintilla::EolMac:
			return QString( "\n" );
			break;
		default:
			return QString( "\r\n" );
	}
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

QString QMakeItem::scope() const
{
	QString s;
	ProjectItem* j = const_cast<QMakeItem*>( this );
	ProjectItem* p = project();
	while ( j && j != p )
	{
		if ( j->isScope() )
			s.prepend( QString( "%1%2" ).arg( j->getValue(), j->getOperator().isEmpty() ? ":" : j->getOperator() ) );
		j = j->parent();
	}
	s.chop( 1 );
	return s;
}

QString QMakeItem::checkScope( const QString& s ) const
{
	QString ss = s.trimmed();
	if ( ss.endsWith( ':' ) )
		ss.chop( 1 );
	return ss;
}

bool QMakeItem::isEqualScope( const QString& s1, const QString& s2 ) const
{
	if ( s2 == "*" )
		return true;
	return checkScope( QString( s1 ).replace( '/', ':' ) ) == checkScope( QString( s2 ).replace( '/', ':' ) );
}

bool QMakeItem::isProjectsContainer() const
{ return isProject() ? getStringValues( "TEMPLATE" ).toLower() == "subdirs" : false; }

void QMakeItem::appendRow( ProjectItem* i )
{ insertRow( rowCount(), i ); }

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
	
	// set project modified
	project()->setModified( true );
}

bool QMakeItem::swapRow( int i, int j )
{
	if ( -1 < i < rowCount() && -1 < j < rowCount() && i != i )
	{
		QList<QStandardItem*> ii;
		QList<QStandardItem*> ij;
		if ( i > j )
		{
			ii = takeRow( i );
			ij = takeRow( j );
		}
		else
		{
			ij = takeRow( j );
			ii = takeRow( i );
		}
		QStandardItem::insertRow( i, ij );
		QStandardItem::insertRow( j, ii );
		return true;
	}
	return false;
}

bool QMakeItem::moveRowUp( int i )
{
	ProjectItem* it = child( i );
	bool b = it && !it->isScopeEnd() && i > 0;
	if ( b )
		QStandardItem::insertRow( i -1, takeRow( i ) );
	return b;
}

bool QMakeItem::moveRowDown( int i )
{
	ProjectItem* it = child( i +1 );
	bool b = it && !it->isScopeEnd() && i < rowCount() -1;
	if ( b )
		QStandardItem::insertRow( i +1, takeRow( i ) );
	return b;
}

bool QMakeItem::moveUp()
{ return parent() ? parent()->moveRowUp( row() ) : false; }

bool QMakeItem::moveDown()
{ return parent()  ? parent()->moveRowDown( row() ) : false; }

void QMakeItem::remove()
{
	if ( parent() )
		parent()->removeRow( row() );
	else if ( model() )
		model()->removeRow( row(), index().parent() );
	else
		Q_ASSERT( 0 );
}

bool QMakeItem::open()
{
	if ( isProject() )
	{
		if ( !QFile::exists( getValue() ) )
			return false;
		
		// populate datas
		QMakeParser p( getValue(), const_cast<QMakeItem*>( this ) );
		
		// set proejct writable
		setReadOnly( false );
		
		// set project unmodified
		setModified( false );
		
		// refresh project
		refresh();
		
		// same for childproject
		foreach ( ProjectItem* p, childrenProjects( true ) )
		{
			p->setReadOnly( false );
			p->setModified( false );
			p->refresh();
		}
		
		return p.isOpen();
	}
	return false;
}

bool QMakeItem::addProject( const QString& s )
{
	if ( !isProjectsContainer() )
		return false;
	
	foreach ( ProjectItem* p, childrenProjects( false ) )
		if ( QFileInfo( p->getValue() ) == QFileInfo( s ) )
			return true;
	
	// create project item
	if ( ProjectItem* it = plugin()->getProjectItem( s ) )
	{
		if ( it->open() )
		{
			addStringValues( relativePath( s ), "SUBDIRS", "+=", "" );
			appendRow( it );
			it->setModified( false );
			return true;
		}
		delete it;
	}
	
	return false;
}

void QMakeItem::editSettings()
{ if ( isProject() ) UIQMakeProjectSettings::instance( this )->exec(); }

void QMakeItem::close()
{
	// only project item can call this
	if ( !isProject() )
		return;
	
	// close subprojects first
	while ( ProjectItem* p = childrenProjects( false ).value( 0 ) )
		p->close();
	
	// tell we will close the project
	emit aboutToClose();
	
	// ask to save project if needed
	save( true );
	
	// tell project is closed
	emit closed();
	
	// remove it from model
	remove();
}

void QMakeItem::save( bool b )
{
	// only project item can call this
	if ( !isProject() )
		return;
	
	if ( getReadOnly() || !getModified() )
		return;
	
	if ( !b || ( b && question( QObject::tr( "Save Project..." ), QObject::tr( "The project [%1] has been modified, save it ?" ).arg( name() ) ) ) )
		writeProject();
}

void QMakeItem::saveAll( bool b )
{
	// save current project
	save( b );
	// save all children projects
	foreach ( ProjectItem* it, childrenProjects() )
		it->save( b );
}

ProjectItem* variable( ProjectItem* s, const QString& var, const QString& op )
{
	Q_ASSERT( s != 0 );
	foreach ( ProjectItem* it, s->children( false, true ) )
		if ( it->getValue() == var && it->getOperator() == op )
			return it;
	ProjectItem* it = new QMakeItem( ProjectItem::VariableType, s );
	it->setValue( var );
	it->setOperator( op );
	it->setMultiLine( true );
	return it;
}

void QMakeItem::addExistingFiles( const QStringList& l, const QString& s, const QString& o )
{ addExistingFiles( l, getItemScope( s, true ), o ); }

void QMakeItem::addExistingFiles( const QStringList& l, ProjectItem* s, const QString& o )
{
	if ( !isProject() )
		return;
	
	// C++ filters
	const QStringList cf = availableSuffixes().value( "C++" );
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
	
	foreach ( const QString& f, l )
	{
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
			if ( question( QObject::tr( "FORMS3 Files..." ), QObject::tr( "Do i need to add this form to FORMS3 variable ?" ) ) )
				vn = "FORMS3";
		}
		else if ( QDir::match( pjf, f ) )
		{
			if ( !isProjectsContainer() )
				warning( QObject::tr( "Add Existing Files..." ), QObject::tr( "This project is not a projects container, cancel adding project:\n%1" ).arg( f ) );
			else if( !addProject( f ) )
				warning( QObject::tr( "Add Existing Files..." ), QObject::tr( "A selected file is a project that i can't open:\n%1" ).arg( f ) );
			continue;
		}
		
		// create value for variable
		ProjectItem* it = new QMakeItem( ProjectItem::ValueType, variable( s, vn, o ) );
		it->setValue( vn != "UNKNOW_FILES" ? relativeFilePath( f ) : f );
	}
}

void QMakeItem::addExistingFile( const QString& f, ProjectItem* s, const QString& o )
{ addExistingFiles( QStringList( f ), s, o ); }

void QMakeItem::addExistingFile( const QString& f, const QString& s, const QString& o )
{ addExistingFile( f, getItemScope( s, true ), o ); }

void QMakeItem::setCompiler( CompilerPlugin* c )
{
	if ( isProject() )
		mCompiler = c;
	else
		project()->setCompiler( c );
}

CompilerPlugin* QMakeItem::compiler() const
{ return isProject() ? mCompiler : project()->compiler(); }

void QMakeItem::setDebugger( DebuggerPlugin* d )
{
	if ( isProject() )
		mDebugger = d;
	else
		project()->setDebugger( d );
}

DebuggerPlugin* QMakeItem::debugger() const
{ return isProject() ? mDebugger : project()->debugger(); }

void QMakeItem::setInterpreter( InterpreterPlugin* i )
{
	if ( isProject() )
		mInterpreter = i;
	else
		project()->setInterpreter( i );
}

InterpreterPlugin* QMakeItem::interpreter() const
{ return isProject() ? mInterpreter : project()->interpreter(); }

void QMakeItem::debug()
{
	QString s;
	foreach ( ProjectItem* it, projectItems() )
		s.append( QString( "%1(%2)%3%4%5" ).arg( QString().fill( ' ', it->parentCount() ) ).arg( it->getType() ).arg( it->getValue() ).arg( it->getOperator() ).arg( it->getEol() ) );
	qWarning( qPrintable( s ) );
}

ProjectItem* QMakeItem::getItemScope( const QString& s, bool c ) const
{
	if ( s.isEmpty() )
		return project();
	ProjectItem* sit = 0;
	foreach ( ProjectItem* it, projectScopes() )
	{
		// good scope
		if ( isEqualScope( it->scope(), s ) )
		{
			if ( it->isNested() )
			{
				it->setType( ProjectItem::ScopeType );
				it->setOperator( QString::null );
			}
			return it;
		}
		// else try found nearest scope
		else if  ( c && checkScope( s ).startsWith( it->scope() ) )
		{
			if ( ( it->isScope() ) && s[it->scope().length()] == ':' )
				if ( !sit || ( sit && sit->scope().length() < it->scope().length() ) )
					sit = it;
		}
	}
	
	// not found and not want create
	if ( !c )
		return 0;
	
	// get nearest scope and remove it from full scope to create
	QString p = s;
	if ( sit )
	{
		p.remove( sit->scope() );
		if ( sit->isScopeEnd() )
		{
			sit->setType( ProjectItem::ScopeType );
			sit->setOperator( QString::null );
		}
	}
	
	// remove trailing operator
	if ( p.startsWith( ':' ) )
		p.remove( 0, 1 );
	
	// split nested : scope
	QStringList l = p.split( ':', QString::SkipEmptyParts );
	
	// create each scope
	foreach ( QString scope, l )
	{
		sit = new QMakeItem( ProjectItem::NestedScopeType, sit ? sit : project() );
		sit ->setValue( scope );
		sit ->setOperator( ":" );
		(void) new QMakeItem( ProjectItem::ScopeEndType, sit );
	}
	
	// return full created scope
	return sit;
}

QStringList QMakeItem::getListValues( const QString& v, const QString& o, const QString& s ) const
{
	QStringList l;
	foreach ( ProjectItem* it, getItemListValues( v, o, s ) )
		l << it->getValue();
	return l;
}

QString QMakeItem::getStringValues( const QString& v, const QString& o, const QString& s ) const
{ return getListValues( v, o, s ).join( " " ); }

void QMakeItem::setListValues( const QStringList& vl, const QString& v, const QString& o, const QString& s )
{
	ProjectItem* it = getItemVariable( v, o, s );
	// create index if it not exists
	if ( !it )
	{
		// if nothing to add, return
		if ( vl.isEmpty() )
			return;
		// create variable
		it = new QMakeItem( ProjectItem::VariableType, getItemScope( s, true ) );
		it->setValue( v );
		it->setOperator( o );
	}
	// set values
	QStringList l = vl;
	// check all child of variable and remove from model/list unneeded index/value
	foreach ( ProjectItem* c, it->children( false, true ) )
	{
		if ( c->isValue() )
		{
			if ( l.contains( c->getValue() ) )
				l.removeAll( c->getValue() );
			else if ( !l.value( 0 ).isEmpty() )
			{
				c->setValue( l.value( 0 ) );
				l.removeFirst();
			}
			else
				c->remove();
		}
	}
	// add require index
	if ( it )
	{
		foreach ( QString e, l )
		{
			if ( !e.isEmpty() )
			{
				ProjectItem* cItem = new QMakeItem( ProjectItem::ValueType, it );
				cItem->setValue( e );
			}
		}
		// if variable is empty, remove it
		if ( !it->rowCount() )
			it->remove();
	}
}

void QMakeItem::setStringValues( const QString& val, const QString& v, const QString& o, const QString& s )
{ setListValues( val.isEmpty() ? QStringList() : QStringList( val ), v, o, s ); }

void QMakeItem::addListValues( const QStringList& vl, const QString& v, const QString& o, const QString& s )
{
	ProjectItem* it = getItemVariable( v, o, s );
	// create index if it not exists
	if ( !it )
	{
		// if nothing to add, return
		if ( vl.isEmpty() )
			return;
		// create variable
		it = new QMakeItem( ProjectItem::VariableType, getItemScope( s, true ) );
		it->setValue( v );
		it->setOperator( o );
	}
	// add values
	QStringList l = vl;
	// check all values of variable and remove from list already existing values
	foreach ( ProjectItem* c, it->children( false, true ) )
		if ( c->isValue() )
			if ( l.contains( c->getValue() ) )
				l.removeAll( c->getValue() );
	// add require values
	if ( it )
	{
		foreach ( QString e, l )
		{
			if ( !e.isEmpty() )
			{
				ProjectItem* cItem = new QMakeItem( ProjectItem::ValueType, it );
				cItem->setValue( e );
			}
		}
	}
}

void QMakeItem::addStringValues( const QString& val, const QString& v, const QString& o, const QString& s )
{ addListValues( QStringList( val ), v, o, s ); }

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
	foreach ( ProjectItem* it, children() )
		if ( it->project() == this )
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

void QMakeItem::writeItem( ProjectItem* it ) //
{
	// cancel if no item
	if ( !it )
		return;
	// write to buffer
	switch ( it->getType() )
	{
		case ProjectItem::EmptyType:
		{
			mBuffer.append( it->getIndent() ).append( it->getEol() );
			break;
		}
		case ProjectItem::FolderType:
		{
			break;
		}
		case ProjectItem::CommentType:
		{
			mBuffer.append( it->getIndent() ).append( it->getValue() ).append( it->getEol() );
			break;
		}
		case ProjectItem::NestedScopeType:
		{
			mBuffer.append( it->getIndent() ).append( it->getValue() ).append( it->getOperator() );
			break;
		}
		case ProjectItem::ScopeType:
		{
			mBuffer.append( it->getIndent() ).append( it->getValue() ).append( " {" ).append( it->getComment().isEmpty() ? "" : it->getComment().prepend( " " ) ).append( it->getEol() );
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
				mBuffer.append( it->getIndent() ).append( "}" ).append( it->getComment().isEmpty() ? "" : it->getComment().prepend( " " ) ).append( p ? " " : it->getEol() );
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
			if ( it->parent()->getMultiLine() )
				mBuffer.append( it->getIndent() ).append( it->getValue() ).append( !it->isLast() ? " \\" : "" ).append( it->getComment().isEmpty() ? "" : it->getComment().prepend( " " ) ).append( it->getEol() );
			else
				mBuffer.append( it->getValue() ).append( it->getComment().isEmpty() ? "" : it->getComment().prepend( " " ) ).append( it->isLast() ? it->getEol() : " " );
			break;
		}
		case ProjectItem::FunctionType:
		{
			mBuffer.append( it->getIndent() ).append( it->getValue() ).append( it->getComment().isEmpty() ? "" : it->getComment().prepend( " " ) ).append( it->getEol() );
			break;
		}
		case ProjectItem::IncludeType:
		{
			mBuffer.append( it->getIndent() ).append( it->getValue() ).append( it->getComment().isEmpty() ? "" : it->getComment().prepend( " " ) ).append( it->getEol() );
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
	ProjectItem* p = it->project();
	foreach ( ProjectItem* cit, it->children() )
		if ( cit->project() == p )
			writeItem( cit );
}
