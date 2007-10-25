/****************************************************************************
**
** 		Created using Monkey Studio v1.8.0.0
** Author    : Azevedo Filipe aka Nox P@sNox <pasnox@gmail.com>
** Project   : UICtags2Api
** FileName  : UICtags2Api.cpp
** Date      : 2007-10-23T21:26:13
** License   : GPL
** Comment   : Your comment here
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/
#include "UICtags2Api.h"
#include "pMonkeyStudio.h"

#include <QRegExp>
#include <QFile>
#include <QBuffer>
#include <QTextCodec>
#include <QProcess>

using namespace pMonkeyStudio;

UICtags2Api::UICtags2Api( QWidget* w )
	: QDialog( w )
{
	setupUi( this );
	pbLoading->setVisible( false );
}

UICtags2Api::~UICtags2Api()
{
	// clear cached files
	mFileCache.clear();
}

QList<QByteArray> UICtags2Api::getFileContent( const QString& s )
{
	if ( mFileCache.contains( s ) )
		return mFileCache[s];
	// create caching file
	QFile f( s );
	if ( !f.open( QFile::ReadOnly | QFile::Text ) )
		return QList<QByteArray>();
	// read lines
	while ( !f.atEnd() )
		mFileCache[s] << f.readLine();
	// return content
	return mFileCache[s];
}

void UICtags2Api::on_tbCtagsBinary_clicked()
{
	QString s = getOpenFileName( tr( "Select the ctags binary file" ), leCtagsBinary->text(), QString::null, this );
	if ( !s.isNull() )
		leCtagsBinary->setText( s );
}

void UICtags2Api::on_tbBrowse_clicked()
{
	QString c, s;
	switch( cbGenerateFrom->currentIndex() )
	{
		case 0: // include
			s = getExistingDirectory( tr( "Select the include path to scan" ), leLabel->text(), this );
			break;
		case 1: // api
			s = getOpenFileName( tr( "Select the api file to prepare" ), leLabel->text(), tr( "Api Files (*.api)" ), this );
			break;
		case 2: // ctags
			s = getOpenFileName( tr( "Select the tags file to convert" ), leLabel->text(), tr( "Ctags File (tags *.tags)" ), this );
			break;
	}
	if ( !s.isNull() )
		leLabel->setText( s );
}

int bracesDiff( const QByteArray& s )
{
	int diff = 0;
	int mode = 0; // 0 <=> default, 1 <=> comment, 2 <=> string
	for ( int i = 0; i < s.count(); i++ )
	{
		if ( mode == 0 ) // default
		{
			if ( s[i] == '(' )
				diff++;
			else if ( s[i] == ')' )
				diff--;
			else if ( s[i] == '"' )
				mode = 2;
			else if ( i > 0 && s[i -1] == '/' && s[i] == '/' )
				return diff;
			else if ( i > 0 && s[i -1] == '/' && s[i] == '*' )
				mode = 1;
		}
		else if ( mode == 1 ) //  comment
		{
			if ( i > 0 && s[i -1] == '*' && s[i] == '/' )
				mode = 0;
		}
		else if ( mode == 2 ) // string
		{
			if ( s[i] == '"' )
				mode = 0;
		}
	}
	return diff;
}

bool UICtags2Api::processCtagsBuffer( const QByteArray& a )
{
	qWarning( a );
	
	if ( a.isEmpty() )
		return false;
	
	QBuffer b;
	b.setData( a );
	if ( !b.open( QBuffer::ReadOnly | QFile::Text ) )
		return false;
	
	// set progress bar
	pbLoading->setVisible( true );
	pbLoading->setValue( 0 );
	pbLoading->setMaximum( a.split( '\n' ).count() -1 );
	
	// process buffer
	QList<QByteArray> lb, contents;
	QByteArray curDef, rt;
	QString c;
	bool removePrivate = true;
	bool winMode = true;
	QString include = "A";
	int braces = 0, curLineNo = 0;
	
	while ( b.canReadLine() )
	{
		// get line
		const QString l = QTextCodec::codecForLocale()->toUnicode( b.readLine() );
		
		// if no comment line
		if ( l.split( '\t' ).count() > 3 && l[0] != '!' )
		{
			// create ctags entity
			CtagsEntity e( l );
			// get line in file
			c = e.getAddress();
			curLineNo = c.left( c.length() -2 ).toInt() -1; // -1 because line numbers in tags file start at 1.
			// cache file
			contents = getFileContent( e.getFile().prepend( QFileInfo( leLabel->text() ).isDir() ? leLabel->text() : QFileInfo( leLabel->text() ).path() ).append( "/" ) );
			// checking entity
			if ( ( !removePrivate || e.getName()[0] != '_' ) && !e.getName().startsWith( "operator " ) )
			{
				// get kind value
				c = e.getKindValue();
				// kind...
				if ( c == "p" || c == "f" )
				{
					curDef = contents[curLineNo];
					braces = bracesDiff( curDef );
					while ( braces > 0 ) // search end of prototype.
					{
						curLineNo++;
						braces = braces +bracesDiff( contents[curLineNo] );
						curDef = curDef +contents[curLineNo];
					}
					// Replace whitespace sequences with a single space character.
					curDef = curDef.simplified();
					// Qt slot
					if ( QString( curDef ).contains( QRegExp( "Q_.*_SLOT" ) ) )
					{
						// cancel because internal member
						curDef.clear();
						//qWarning( QString( curDef ).replace( QRegExp( "^Q_PRIVATE_SLOT\\(.*,(.*)\\)" ), "\\1" ).toAscii() );
					}
					// Remove space around the '('.
					curDef.replace( " (", "(" ).replace( "( ", "(" );
					// Remove trailing semicolon.
					curDef.replace( ";", "" );
					// Remove implementation if present.
					if ( curDef.contains( "{" ) )
					{
						if ( curDef.contains( "}" ) )
						{
							int startImpl = curDef.indexOf( "{" );
							int endImpl = curDef.indexOf( "}" );
							curDef.remove( startImpl, endImpl -startImpl +1 );
						}
						else
						{
							// Remove trailing brace.
							curDef.remove( curDef.lastIndexOf( "{" ), 1 );
						}
					}
					// get return type.
					rt = curDef.mid( 0, curDef.indexOf( e.getName() ) ).simplified();
					// remove return type.
					curDef = curDef.mid( curDef.indexOf( e.getName() ) ).trimmed();
					// remove final comment
					if ( curDef.contains( "//" ) )
					{
						int cs = curDef.indexOf( "//" );
						int bs = curDef.indexOf( ")" );
						if ( cs > bs )
							curDef = curDef.left( cs ).trimmed();
					}
					// Remove virtual indicator.
					if ( curDef.trimmed().replace( " ", "" ).endsWith( ")=0" ) )
					{
						curDef.remove( curDef.lastIndexOf( "0" ), 1 );
						curDef.remove( curDef.lastIndexOf( "=" ), 1 );
					}
					// Remove trailing space.
					curDef = curDef.trimmed();
					// winmode
					if ( winMode )
					{
						if ( curDef.contains( "A(" ) && include == "A" )
							curDef.replace( "A(", "(" );
						else if ( curDef.contains( "W(" ) && include == "W" )
							curDef.replace( "W(", "(" );
					}
				}
				else if ( c == "d" )
				{
					if ( !winMode || ( !e.getName().endsWith( "A" ) && !e.getName().endsWith( "W" ) ) )
						curDef = e.getName().toAscii();
				}
				else
					curDef = e.getName().toAscii();
				// prepend context if available
				if ( !e.getFieldValue( "class" ).isEmpty() )
					curDef.prepend( e.getFieldValue( "class" ).append( "::" ).toAscii() );
				else if ( !e.getFieldValue( "struct" ).isEmpty() )
					curDef.prepend( e.getFieldValue( "struct" ).append( "::" ).toAscii() );
				else if ( !e.getFieldValue( "namespace" ).isEmpty() )
					curDef.prepend( e.getFieldValue( "namespace" ).append( "::" ).toAscii() );
				else if ( !e.getFieldValue( "enum" ).isEmpty() )
					curDef.prepend( e.getFieldValue( "enum" ).append( "::" ).toAscii() );
				// check return type
				if ( !rt.isEmpty() )
				{
					// remove Qt export macro
					rt = QString( rt ).replace( QRegExp( "Q_.*_EXPORT" ), "" ).trimmed().toAscii();
					curDef.append( " (" +rt +")" );
				}
				
				// append to buffer if needed
				if ( !curDef.isEmpty() && !lb.contains( curDef ) )
					lb << curDef;
			}
		}
		
		// clear bytearrays
		curDef.clear();
		rt.clear();
		
		// increase progressbar
		pbLoading->setValue( pbLoading->value() +1 );
		QApplication::processEvents();
	}
	
	if ( lb.isEmpty() )
	{
		pbLoading->setVisible( false );
		return false;
	}
	
	// get save filename
	QString s = getSaveFileName( tr( "Save api file" ), QString::null, tr( "Api Files (*.api)" ), this );
	if ( s.isEmpty() )
	{
		pbLoading->setVisible( false );
		return false;
	}
	
	// save file
	QFile f( s );
	if ( f.open( QFile::WriteOnly ) )
	{
		f.resize( 0 );
		qSort( lb );
		foreach ( QByteArray ba, lb )
			f.write( ba +'\n' );
	}
	else
	{
		pbLoading->setVisible( false );
		return false;
	}
	
	// hide progress bar
	pbLoading->setVisible( false );
	
	// success
	return true;
}

bool UICtags2Api::processCtags( const QString& s )
{
	// create process
	QProcess p;
	p.setProcessChannelMode( QProcess::MergedChannels );
	p.setWorkingDirectory( s );
	// start process
	p.start( QString( "%1 -f - -R -u -n --c-types=pcdgstue ./" ).arg( leCtagsBinary->text() ), QIODevice::ReadOnly | QIODevice::Text );
	// wait process end
	if ( !p.waitForFinished( -1 ) )
		return false;
	// process buffer
	QByteArray a = p.readAll();
	return processCtagsBuffer( a );
}

bool UICtags2Api::processApi( const QString& s )
{
	if ( !QFile::exists( s ) )
		return false;
	return false;
}

bool UICtags2Api::processCtags2Api( const QString& s )
{
	QFile f( s );
	if ( !f.open( QFile::ReadOnly ) )
		return false;
	return processCtagsBuffer( f.readAll() );
}

void UICtags2Api::accept()
{
	setEnabled( false );
	bool b = false;
	switch( cbGenerateFrom->currentIndex() )
	{
		case 0: // include
			b = processCtags( leLabel->text() );
			break;
		case 1: // api
			b = processApi( leLabel->text() );
			break;
		case 2: // ctags
			b = processCtags2Api( leLabel->text() );
			break;
	}
	setEnabled( true );
	if ( b )
		QDialog::accept();
}
