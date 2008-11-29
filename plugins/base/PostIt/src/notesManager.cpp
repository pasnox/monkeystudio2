#include "notesManager.h"

#include <pMonkeyStudio.h>
#include <QFile>
#include <QTextStream>
#include <QStringList>
#include <QTextCodec>
#include <QMessageBox>


//constructor
notesManager::notesManager(QString filepath)
            :mFilepath( filepath ) {}

//destructor
notesManager::~notesManager() {}

//read xmlfile and load mXmlDocument
bool notesManager::readDocument()
{
    QFile file( mFilepath );
   
    if ( file.open( QIODevice::ReadOnly ) && mXmlDocument.setContent( &file ) ) {
        return true;
    } else {
        //insert header
        QDomNode n = mXmlDocument.createProcessingInstruction( "xml", "version=\"1.0\"" );
        mXmlDocument.insertBefore( n , mXmlDocument.firstChild() );
        
        //insert <notes> tag
        QDomElement e = mXmlDocument.createElement( "notes" );
        mXmlDocument.appendChild( e );
    }
    return false;
}

//write xmlFile with mXmlDocument content
bool notesManager::writeDocument()
{
    QFile file( mFilepath );
    bool saved = false;

    file.open( QIODevice::WriteOnly );
    QTextCodec *codec = QTextCodec::codecForName( pMonkeyStudio::defaultCodec().toUtf8() );
    QByteArray datas = codec->fromUnicode( mXmlDocument.toString() );
    file.resize( 0 );
    saved = file.write( datas ) != -1;
    file.close();

    return saved;
}

//return "id" node of mXmlDocument
QDomElement notesManager::getElement( uint id )
{
    QDomNodeList list = mXmlDocument.elementsByTagName( "note" );

    if ( id >= 0 && id < list.length() )
        return list.item( id ).toElement();
    else
        return QDomElement();
}

//add QDomElement with "note" value in mXmlDocument
void notesManager::addElement(QString title, QString note)
{
    QDomElement e = mXmlDocument.createElement( "note" );
    e.appendChild( mXmlDocument.createTextNode( note ) );
    e.setAttribute( "title", title );
    e.setNodeValue( "" );
    mXmlDocument.firstChildElement( "notes" ).appendChild( e );
}

//return number of "note" in document
uint notesManager::getNotesCount()
{
    return mXmlDocument.firstChildElement( "notes" ).childNodes().length();
}

//return list of notes in QStringList
QStringList notesManager::getElements()
{
    QStringList listNote;
    QDomNodeList l = mXmlDocument.elementsByTagName( "note" );

    for ( uint i = 0; i < l.length(); ++i )
        listNote << l.item( i ).toElement().attribute( "title" );

    return listNote;
}

//delete node number "id"
void notesManager::removeElement( uint id )
{
    QDomNode n = this->getElement( id );
    mXmlDocument.firstChildElement( "notes" ).removeChild( n );
}

//delete all nodes
void notesManager::removeAllElements()
{
    QDomNode notes = mXmlDocument.firstChildElement( "notes" );
    QDomNodeList nodes = notes.childNodes();
    
    while ( nodes.count() > 0 )
        notes.removeChild( nodes.at( 0 ) );
}

//change title in "id" note
void notesManager::setTitleElement( uint id, QString title )
{
    QDomElement e = this->getElement( id );
    if ( !e.isNull() ) {
        QDomAttr a = e.toElement().attributeNode( "title" );
        a.setValue( title );
    }
}

//set new note in "id" node
void notesManager::setElement( uint id, QString note )
{
    QDomElement e = this->getElement( id );
    if ( !e.isNull() )
        e.firstChild().toText().setData( note );
}
