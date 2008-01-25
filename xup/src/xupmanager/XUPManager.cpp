#include "XUPManager.h"
#include "ProjectItem.h"

#include <QFile>

QString tr( const char* s, const char* c = 0, int n = -1 )
{ return QObject::tr( s, c, n ); }

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
	pi->setDomDocument( d );
	pi->setDomElement( e );
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
	return f.write( pi->domDocument().toByteArray() ) != -1;
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
			// assign node
			it->setDomElement( de );
			// append item to parent
			p->appendRow( it );
			// parse scope
			parseRoot( it, de );
		}
	}
}
