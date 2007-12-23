#include "XUPManager.h"
#include "ProjectItem.h"

#include <QFile>

QStringList mTextTypes = QStringList() << "comment" << "value" << "emptyline" << "function";
QStringList mFileVariables = QStringList();
QStringList mPathVariables = QStringList();
QHash<QString, QString> mVariableLabels = QHash<QString, QString>();
QHash<QString, QIcon> mVariableIcons = QHash<QString, QIcon>();

bool XUPManager::loadXUP( ProjectItem* pi, const QByteArray& a, const QString& v )
{
	// get document
	QDomDocument d;
	// set document content
	if ( !d.setContent( a ) )
		return false;
	// get project node
	QDomElement e  = d.firstChildElement( "project" ).toElement();
	// check project available
	if ( e.isNull() )
		return false;
	// set project document and element
	pi->setDomElement( e.cloneNode( false ).toElement() );
	pi->setValue( "version", v );
	// parse project scope
	parseRoot( pi, e );
	// return state
	return true;
}

bool XUPManager::loadXUP( ProjectItem* pi, const QString& s, const QString& v )
{
	// check file exists
	if ( !pi || !QFile::exists( s ) )
		return false;
	// open file
	QFile f( s );
	if ( !f.open( QIODevice::ReadOnly ) )
		return false;
	// load xup buffer
	return loadXUP( pi, f.readAll(), v );
}

bool XUPManager::saveXUP( ProjectItem* pi, const QString& s, const QString& v )
{
	// check valid item
	if ( !pi )
		return false;
	// try open file for writing
	QFile f( s );
	if ( !f.open( QIODevice::WriteOnly ) )
		return false;
	// erase file content
	f.resize( 0 );
	// set project version
	pi->setValue( "version", v );
	// write content
	return f.write( pi->toDomDocument().toByteArray() ) != -1;
}

void XUPManager::parseRoot( ProjectItem* p, QDomElement e )
{
	// get nodes
	QDomNodeList l = e.childNodes();
	// iterate over nodes
	for ( int i = 0; i < l.count(); i++ )
	{
		// test not a text node
		if ( !l.at( i ).isText() )
		{
			// get child node
			QDomElement de = l.at( i ).toElement();
			// create item
			ProjectItem* it = p->clone( false );
			// append item to parent
			p->appendRow( it );
			// assign node
			it->setDomElement( de.cloneNode( textTypes().contains( de.tagName() ) ).toElement() );
			// parse scope
			parseRoot( it, de );
		}
	}
}

void XUPManager::recursiveNodes( QDomNode& n, ProjectItem* it )
{
	QDomNode dn = n.appendChild( it->domElement() );
	foreach ( ProjectItem* cit, it->children( false, true ) )
		recursiveNodes( dn, dynamic_cast<ProjectItem*>( cit ) );
}

QDomDocument XUPManager::toDomDocument( ProjectItem* it )
{
	QDomDocument d( "XUPProject" );
	recursiveNodes( d, it );
	return d;
}

QStringList XUPManager::textTypes()
{ return mTextTypes; }

void XUPManager::registerTextType( const QString& s )
{
	if ( !mTextTypes.contains( s ) )
		mTextTypes << s;
}

QStringList XUPManager::fileVariables()
{ return mFileVariables; }

void XUPManager::registerFileVariables( const QString& s )
{
	if ( !mFileVariables.contains( s ) )
		mFileVariables << s;
}

QStringList XUPManager::pathVariables()
{ return mPathVariables; }

void XUPManager::registerPathVariables( const QString& s )
{
	if ( !mPathVariables.contains( s ) )
		mPathVariables << s;
}

QHash<QString, QString> XUPManager::variableLabels()
{ return mVariableLabels; }

void XUPManager::registerVariableLabels( const QString& v, const QString& l )
{ mVariableLabels[v] = l; }

QHash<QString, QIcon> XUPManager::variableIcons()
{ return mVariableIcons; }

void XUPManager::registerVariableIcons( const QString& v, const QIcon& i )
{ mVariableIcons[v] = i; }

QIcon XUPManager::getIcon( const QString& o, const QString& d )
{ return QIcon( QFile::exists( o ) ? o : QString( ":/icons/icons/%1.png" ).arg( d ) ); }

QString XUPManager::valueName( const QString& s )
{
	if ( mTextTypes.contains( s ) )
		return "text";
	return "name";
}

void XUPManager::updateItem( ProjectItem* it )
{
	// set icon
	it->setIcon( getIcon( it->value( "icon" ), it->value( "type" ) ) );
	// set caption
	it->setText( it->defaultValue() );
	// get element tagname
	QString tn = it->value( "type" );
	// set visual datas
	if ( tn == "project" )
	{
		// set ToolTip
		it->setToolTip( QString( "<b>Project</b><br />Name : %1<br />File Path : %2" ).arg( it->defaultValue() ).arg( it->projectFilePath() ) );
	}
	else if ( tn == "comment" )
	{
		// set ToolTip
		it->setToolTip( QString( "<b>Comment</b><br />%1" ).arg( it->defaultValue() ) );
	}
	else if ( tn == "emptyline" )
	{
		// set caption
		it->setText( QString( "%1 Empty Line(s)" ).arg( it->defaultValue() ) );
		// set ToolTip
		it->setToolTip( QString( "<b>Empty Line(s)</b><br />Count : %1" ).arg( it->defaultValue() ) );
	}
	else if ( tn == "variable" )
	{
		// set icon
		QIcon i = QFile::exists( it->value( "icon" ) ) ? QIcon( it->value( "icon" ) ) : variableIcons().value( it->value( "name" ) );
		if ( i.isNull() )
			i = getIcon( QString(), it->value( "type" ) );
		it->setIcon( i );
		// set caption
		if ( variableLabels().contains( it->defaultValue() ) )
			it->setText( variableLabels().value( it->defaultValue() ) );
		// set ToolTip
		it->setToolTip( QString( "<b>Variable</b><br />Name : %1<br />Operator : %2<br />MultiLine : %3" ).arg( it->defaultValue() ).arg( it->value( "operator", "=" ) ).arg( QVariant( it->value( "multiline", "false" ) ).toBool() ? "True" : "False" ) );
	}
	else if ( tn == "value" )
	{
		// set caption
		if ( it->parent() && fileVariables().contains( it->parent()->defaultValue() ) )
			it->setText( QFileInfo( it->defaultValue() ).fileName() );
		// set ToolTip
		it->setToolTip( QString( "<b>Value</b><br />Content : %1<br />Comment : %2" ).arg( it->defaultValue() ).arg( it->value( "comment" ) ) );
	}
	else if ( tn == "function" )
	{
		// set ToolTip
		it->setToolTip( QString( "<b>Function</b><br />Content : %1<br />Comment : %2" ).arg( it->defaultValue() ).arg( it->value( "comment" ) ) );
	}
	else if ( tn == "scope" )
	{
		// set ToolTip
		it->setToolTip( QString( "<b>Scope</b><br />Name : %1<br />Nested : %2" ).arg( it->defaultValue() ).arg( QVariant( it->value( "nested", "false" ) ).toBool() ? "True" : "False" ) );
	}
}
