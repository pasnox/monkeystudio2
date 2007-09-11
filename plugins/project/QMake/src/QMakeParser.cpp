#include "QMakeParser.h"
#include "QMakeItem.h"
#include "UISettingsQMake.h"

#include <QFileInfo>
#include <QTextStream>
#include <QTextCodec>

#include <QVector>
#include <QRegExp>
#include <QStringList>
#include <QDebug>

static int prof = 0; // profondeur courante
static int this_prof = 0; // profondeur precedent

static QVector<QString> content;
QStringList file_variables;
static bool mParseCommands = false;
static QRegExp function_call("^((?:[a-zA-Z0-9\\.]+(?:\\((?:.*)\\))?(?:[ \\t]+)?[|:](?:[ \\t]+)?)+)?([a-zA-Z]+\\((.*)\\))[ \\t]*(#.*)?"); 
static QRegExp bloc("^(\\})?[ \\t]*(?:((?:[-\\.a-zA-Z0-9*|_!+]+(?:\\((?:[^\\)]*)\\))?[:|])+)?([-a-zA-Z0-9*|_!+]+(?:\\((?:[^\\)]*)\\))?))[ \\t]*(\\{)[ \\t]*(#.*)?"); 
static QRegExp variable("^(?:((?:[-\\.a-zA-Z0-9*!_|+]+(?:\\((?:.*)\\))?(?:[ \\t]+)?[:|](?:[ \\t]+)?)+)?([\\.a-zA-Z0-9*!_]+))[ \\t]*([~*+-]?=)[ \\t]*(.*)[ \\t]*(\\\\)?[ \t]*(#.*)?");
//static QRegExp variable("^(?:((?:[-\\.a-zA-Z0-9*!_|+]+(?:\\((?:.*)\\))?(?:[ \\t]+)?[:|](?:[ \\t]+)?)+)?([\\.a-zA-Z0-9*!_]+))[ \\t]*([~*+-]?=)[ \\t]*([^\\\\#]*)[ \\t]*(\\\\)?[ \t]*(#.*)?");
static QRegExp varLine("^[ \\t]*(.*)[ \\t]*\\\\[ \\t]*(#.*)?");
static QRegExp end_bloc("^(\\})[ \t]*(#.*)?");
static QRegExp comments("^\\s*#(.*)");
static QRegExp splitNested( "\\s*([^:|()]+|!?\\w+\\(.*\\))\\s*(:|\\|)" );
static QRegExp splitValues( "([^\\s\"]+)|\\\"([^\"]+)\\\"|(\\${1,2}\\w+\\([^\\(\\)]+\\)[^\\s]+)" );
static QRegExp splitCommands( "(\\([^;]*\\));?|(\\$\\(\\w+\\)[^;]*);?" );
static QRegExp encoding( "[ \\t]*ENCODING[ \\t]*=[ \\t]*([^ ]+)[ \\t]*(?:#.*)?", Qt::CaseInsensitive );

QMakeParser::QMakeParser( const QString& s, QMakeItem* i )
	: mIsOpen( false ), mRoot( i )
{
	// get file/path base variable
	file_variables = UISettingsQMake::readPathFiles();
	// parse file
	loadFile( s, mRoot );
}

QMakeParser::~QMakeParser()
{}

bool QMakeParser::isOpen() const
{ return mIsOpen; }

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
	/*
	if ( !c )
		c = mModel->defaultCodec();
	*/
	// reset file
	f.reset();
	// get decoded stream
	QTextStream t( &f );
	t.setCodec( c );
	// trim file
	while ( !t.atEnd() )
		content += t.readLine().trimmed();
	// set project data
	it->setType( ProjectsModel::ProjectType );
	it->setValue( s );
	// parse buffer
	mIsOpen = parseBuffer( 0, it );
	
	// open subdirs project
	if ( mIsOpen )
		foreach ( ProjectItem* sit, it->getItemListValues( "subdirs", "", "" ) )
			if ( QFile::exists( sit->getFilePath() ) )
				loadFile( sit->getFilePath(), reinterpret_cast<QMakeItem*>( it->clone( ProjectsModel::ProjectType, it ) ) );
			else
				qWarning( "Can't open subproject: %s", qPrintable( sit->getFilePath() ) );
	
	// return root project is open
	return mIsOpen;
}

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
				i->setComment( liste[3].trimmed() );
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
				i->setComment( liste[5].trimmed() );
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
			bool extractValues = file_variables.contains( liste[2] , Qt::CaseInsensitive );
			QMakeItem *i, *v = processNested( liste[1], it );
			v = addVariable( liste[2], liste[3], v );
			v->setMultiLine( liste[5].trimmed() == "\\" );
			i = !extractValues ? addValue( liste[4], v ) : processValues( liste[4], v );
			if ( i )
				i->setComment( liste[6].trimmed() );
			// if last char is \ read next line
			if ( liste[5] == "\\" )
			{
				ligne++;
				// while last char of line is \ continue to read, be carefull on comment
				while( varLine.exactMatch( content[ligne] ) )
				{
					// ==================        5         ==================
					liste = varLine.capturedTexts();
					i = !extractValues ? addValue( liste[1], v ) : processValues( liste[1], v );
					if ( i )
						i->setComment( liste[2].trimmed() );
					ligne++;
				}
				// last line , optionnally with comment
				liste = content[ligne].split( "#" );
				i = !extractValues ? addValue( liste[0], v ) : processValues( liste[0], v );
				if ( i && liste.count() > 1 )
					i->setComment( liste[1] );
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

QMakeItem* QMakeParser::addScope( const QString& v, const QString& o, bool b, QMakeItem* i )
{
	QMakeItem* s = new QMakeItem( b ? ProjectsModel::NestedScopeType : ProjectsModel::ScopeType, i );
	s->setValue( v.trimmed() );
	s->setOperator( o.trimmed() );
	(void) new QMakeItem( ProjectsModel::ScopeEndType, s );
	return s;
}

QMakeItem* QMakeParser::addFunction( const QString& s, const QString& o, QMakeItem* i )
{
	QMakeItem* f = new QMakeItem( ProjectsModel::FunctionType, i );
	f->setValue( s.trimmed() );
	f->setOperator( o.trimmed() );
	return f;
}

QMakeItem* QMakeParser::addVariable( const QString& s, const QString& o, QMakeItem* i )
{
	QMakeItem* v = new QMakeItem( ProjectsModel::VariableType, i );
	v->setValue( s.trimmed() );
	v->setOperator( o.trimmed() );
	mParseCommands = s.trimmed().endsWith( ".commands", Qt::CaseInsensitive );
	return v;
}

QMakeItem* QMakeParser::addValue( const QString& s, QMakeItem* i )
{
	QMakeItem* v = new QMakeItem( ProjectsModel::ValueType, i );
	v->setValue( s.trimmed() );
	return v;
}

QMakeItem* QMakeParser::addComment( const QString& s, QMakeItem* i )
{
	QMakeItem* c = new QMakeItem( ProjectsModel::CommentType, i );
	c->setValue( s.trimmed() );
	return c;
}

QMakeItem* QMakeParser::addEmpty( QMakeItem* i )
{
	QMakeItem* e = new QMakeItem( ProjectsModel::EmptyType, i );
	e->setValue( "" );
	return e;
}
