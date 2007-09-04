#include "QMakeParser.h"
#include "QMakeItem.h"

#include <QFileInfo>
#include <QTextStream>
#include <QTextCodec>

#include <QVector>
#include <QRegExp>
#include <QStringList>
#include <QDebug>

static int prof = 0; // profondeur courrante
static int this_prof = 0; // profondeur precedent

static QVector<QString> content;
static bool mParseCommands = false;
static QRegExp function_call("^((?:[a-zA-Z0-9\\.]+(?:\\((?:.*)\\))?(?:[ \\t]+)?[|:](?:[ \\t]+)?)+)?([a-zA-Z]+\\((.*)\\))[ \\t]*(#.*)?"); 
static QRegExp bloc("^(\\})?[ \\t]*(?:((?:[-\\.a-zA-Z0-9*|_!+]+(?:\\((?:[^\\)]*)\\))?[:|])+)?([-a-zA-Z0-9*|_!+]+(?:\\((?:[^\\)]*)\\))?))[ \\t]*(\\{)[ \\t]*(#.*)?"); 
static QRegExp variable("^(?:((?:[-\\.a-zA-Z0-9*!_|+]+(?:\\((?:.*)\\))?(?:[ \\t]+)?[:|](?:[ \\t]+)?)+)?([\\.a-zA-Z0-9*!_]+))[ \\t]*([~*+-]?=)[ \\t]*([^\\\\#]*)[ \\t]*(\\\\)?[ \t]*(#.*)?");
static QRegExp varLine("^[ \\t]*(.*)[ \\t]*\\\\[ \\t]*(#.*)?");
static QRegExp end_bloc("^(\\})[ \t]*(#.*)?");
static QRegExp comments("^\\s*#(.*)");
static QRegExp splitNested( "\\s*([^:|()]+|!?\\w+\\(.*\\))\\s*(:|\\|)" );
static QRegExp splitValues( "([^\\s\"]+)|\\\"([^\"]+)\\\"|(\\${1,2}\\w+\\([^\\(\\)]+\\)[^\\s]+)" );
static QRegExp splitCommands( "(\\([^;]*\\));?|(\\$\\(\\w+\\)[^;]*);?" );
static QRegExp encoding( "[ \\t]*ENCODING[ \\t]*=[ \\t]*([^ ]+)[ \\t]*(?:#.*)?", Qt::CaseInsensitive );

QMakeParser::QMakeParser( const QString& s, QMakeItem* i )
	: QObject( i ), mIsOpen( false ), mRoot( i )
{
	loadFile( s, mRoot );
}
//
QMakeParser::~QMakeParser()
{
}
//
bool QMakeParser::isOpen() const
{
	return mIsOpen;
}
//
bool QMakeParser::loadFile( const QString& s, QMakeItem* it )
{
	// clear vectors
	content.clear();
	// open file
	QFile f( s );
	if ( !f.exists() || !f.open( QFile::ReadOnly | QFile::Text ) )
		return false;
	// loking for encoding
	QTextCodec* c = 0;
	if ( encoding.indexIn( f.readAll() ) != -1 )
		c = QTextCodec::codecForName( encoding.capturedTexts().at( 1 ).trimmed().toAscii() );
	if ( !c )
		c = mModel->defaultCodec();
	// reset file
	f.reset();
	// get decoded stream
	QTextStream t( &f );
	t.setCodec( c );
	// trim file
	while ( !t.atEnd() )
		content += t.readLine().trimmed();
	// set project data
	it->setType( AbstractProjectModel::ProjectType );
	it->setData( QFileInfo( s ).completeBaseName() );
	it->setData( s, AbstractProjectModel::AbsoluteFilePathRole );
	it->setData( false, AbstractProjectModel::ProjectModifiedRole );
	// parse buffer
	mIsOpen = parseBuffer( 0, it );
	// open subdirs project
	if ( mIsOpen )
		foreach ( QModelIndex i, mModel->getIndexListValues( "subdirs", it->index(), "" ) )
		{
			// get subproject filepath and stock it
			QString sp  = i.data( AbstractProjectModel::ValueRole ).toString();
			sp = QString( "%1/%2.pro" ).arg( mModel->filePath( sp, i ) ).arg( QFileInfo( sp ).fileName() );
			if ( QFile::exists( sp ) )
			{
				mModel->setData( i, sp, AbstractProjectModel::AbsoluteFilePathRole );
				loadFile( sp, new QMakeItem( AbstractProjectModel::ProjectType, it ) );
			}
			else
				qWarning( "Can't open subproject: %s", qPrintable( sp ) );
		}
	return mIsOpen;
}
// parser file
int QMakeParser::parseBuffer( int ligne, QMakeItem* it )
{
	while(ligne < content.size())
	{
		// function like :
		// func_name(params), nested optionnally
		if ( function_call.exactMatch( content[ligne] ) )
		{
			// ==================        1        ==================
			QStringList liste = function_call.capturedTexts();
			QMakeItem* i = processNested( liste[1], it );
			i = addFunction( liste[2].trimmed(), "", i );
			if ( i )
				i->setData( liste[3].trimmed(), AbstractProjectModel::CommentRole );
		}
		// scope (nested compris)
		// "truc(params) {" ou "xml:truc(params) {" ("{" facultatif)
		else if ( bloc.exactMatch( content[ligne] ) )
		{
			// if end block and start new one
			if( content[ligne].left(1) == "}" )
			{
				// drop infinite loop
				if(this_prof != prof)
				{
					this_prof = prof-1;
					return ligne-1;
				}
			}
			// ==================        2         ==================
			QStringList liste = bloc.capturedTexts();
			QMakeItem* i = processNested( liste[2], it );
			i = addScope( liste[3].trimmed(), "", false, i );
			if ( i )
				i->setData( liste[5].trimmed(), AbstractProjectModel::CommentRole );
			//
			prof++;
			// parse block giving parent scope
			ligne = parseBuffer( ligne+1, i );
			prof--;
		}
		// end of block
		else if ( end_bloc.exactMatch( content[ligne] ) )
			return ligne;
		// variable ( nested optionnaly)
		// "HEADERS = ***" or "win32:HEADERS = ***" ("+=" and "-=" managed)
		else if ( variable.exactMatch( content[ligne] ) )
		{
			// ==================        4         ==================
			QStringList liste = variable.capturedTexts();
			QMakeItem *i, *v = processNested( liste[1], it );
			v = addVariable( liste[2], liste[3], v );
			v->setData( liste[5].trimmed() == "\\", AbstractProjectModel::MultiLineRole );
			i = processValues( liste[4], v );
			if ( i )
				i->setData( liste[6].trimmed(), AbstractProjectModel::CommentRole );
			// if last char is \ read next lines
			if ( liste[5] == "\\" )
			{
				ligne++;
				// while last char of line is \ continue to read, be carrefull on comment
				while( varLine.exactMatch( content[ligne] ) )
				{
					// ==================        5         ==================
					liste = varLine.capturedTexts();
					i = processValues( liste[1], v );
					if ( i )
						i->setData( liste[2].trimmed(), AbstractProjectModel::CommentRole );
					ligne++;
				}
				// last line , optionnally with comment
				liste = content[ligne].split( "#" );
				i = processValues( liste[0], v );
				if ( i && liste.count() > 1 )
					i->setData( liste[1], AbstractProjectModel::CommentRole );
			}
		}
		// single line comment
		else if ( comments.exactMatch( content[ligne] ) )
			addComment( content[ligne], it );
		// empty lines
		else if ( content[ligne].trimmed() == "" )
			addEmpty( it );
		// error
		else
			qWarning( "Error on line %d: %s", (ligne+1), qPrintable( content[ligne] ) );
		ligne++;
	}
	return 1;
}
// 100%
QMakeItem* QMakeParser::processNested( const QString& s, QMakeItem* i )
{
	QStringList l;
	int p = 0;
	while ( ( p = splitNested.indexIn( s, p ) ) != -1 )
	{
		l = splitNested.capturedTexts();
		i = addScope( l.at( 1 ), l.at( 2 ), true, i );
		p += splitNested.matchedLength();
	}
	return i;
}
// 100%
QMakeItem* QMakeParser::processValues( const QString& s, QMakeItem* i )
{
	QStringList l;
	int p = 0;
	QMakeItem* v = 0;
	QRegExp* r = mParseCommands ? &splitCommands : &splitValues;
	while ( ( p = r->indexIn( s, p ) ) != -1 )
	{
		l = r->capturedTexts();
		l.removeAll( "" );
		v = addValue( l.at( 1 ), i );
		p += r->matchedLength();
	}
	return v;
}
// 100%
QMakeItem* QMakeParser::addScope( const QString& v, const QString& o, bool b, QMakeItem* i )
{
	QMakeItem* s = new QMakeItem( b ? AbstractProjectModel::NestedScopeType : AbstractProjectModel::ScopeType, i );
	s->setData( v.trimmed(), AbstractProjectModel::ValueRole );
	s->setData( o.trimmed(), AbstractProjectModel::OperatorRole );
	(void) new QMakeItem( AbstractProjectModel::ScopeEndType, s );
	return s;
}
// 100%
QMakeItem* QMakeParser::addFunction( const QString& s, const QString& o, QMakeItem* i )
{
	QMakeItem* f = new QMakeItem( AbstractProjectModel::FunctionType, i );
	f->setData( s.trimmed(), AbstractProjectModel::ValueRole );
	f->setData( o.trimmed(), AbstractProjectModel::OperatorRole );
	return f;
}
// 100%
QMakeItem* QMakeParser::addVariable( const QString& s, const QString& o, QMakeItem* i )
{
	QMakeItem* v = new QMakeItem( AbstractProjectModel::VariableType, i );
	v->setData( s.trimmed(), AbstractProjectModel::ValueRole );
	v->setData( o.trimmed(), AbstractProjectModel::OperatorRole );
	mParseCommands = s.trimmed().endsWith( ".commands", Qt::CaseInsensitive );
	return v;
}
// 100%
QMakeItem* QMakeParser::addValue( const QString& s, QMakeItem* i )
{
	QMakeItem* v = new QMakeItem( AbstractProjectModel::ValueType, i );
	v->setData( s.trimmed(), AbstractProjectModel::ValueRole );
	return v;
}
// 100%
QMakeItem* QMakeParser::addComment( const QString& s, QMakeItem* i )
{
	QMakeItem* c = new QMakeItem( AbstractProjectModel::CommentType, i );
	c->setData( s.trimmed(), AbstractProjectModel::ValueRole );
	return c;
}
// 100%
QMakeItem* QMakeParser::addEmpty( QMakeItem* i )
{
	QMakeItem* e = new QMakeItem( AbstractProjectModel::EmptyType, i );
	e->setData( "", AbstractProjectModel::ValueRole );
	return e;
}
