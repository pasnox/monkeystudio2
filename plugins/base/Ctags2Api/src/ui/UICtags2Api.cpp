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
#include <QByteArray>
#include <QBuffer>
#include <QTextCodec>
#include <QHash>

using namespace pMonkeyStudio;

UICtags2Api::UICtags2Api( QWidget* w )
	: QDialog( w )
{
	setupUi( this );
	pbLoading->setVisible( false );
}

UICtags2Api::~UICtags2Api()
{}

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

int bracesDiff( const QString& s )
{
	int diff = 0;
	int mode = 0; // 0 <=> default, 1 <=> comment, 2 <=> string
	for ( int i = 0; i < s.count(); i++ )
	{
		if ( mode == 0 ) // default
		{
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
	if ( a.isEmpty() )
		return false;
	
	QBuffer b;
	b.setData( a );
	if ( !b.open( QBuffer::ReadOnly | QFile::Text ) )
		return false;
	
	// output buffer
	QList<QByteArray> lb;
	
	// set progress bar
	pbLoading->setVisible( true );
	pbLoading->setValue( 0 );
	pbLoading->setMaximum( a.split( '\n' ).count() -1 );
	
	// process buffer
	QString s, c, rt;
	bool removePrivate = true;
	bool winMode = true;
	QString include = "A";
	
	while ( b.canReadLine() )
	{
		// get line
		const QString l = QTextCodec::codecForLocale()->toUnicode( b.readLine() );
		
		// if no comment line
		if ( l[0] != '!' )
		{
			// create ctags entity
			CtagsEntity e( l );
			
			// checking entity
			if ( ( !removePrivate || e.getName()[0] != '_' ) && !e.getName().startsWith( "operator " ) )
			{
				c = e.getKindValue();
				if ( c == "p" || c == "f" )
				{
					QString curDef = e.getAddress();
					// remove regexp begin
					curDef.replace( "/^", "" );
					// Remove trailing semicolon and quote.
					curDef.replace( ";\"", "" );
					// remove regexp end
					curDef.replace( "$/", "" );
					// Replace whitespace sequences with a single space character.
					curDef = curDef.simplified();
					// Remove space around the '('.
					curDef.replace( " (", "(" ).replace( "( ", "(" );
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
					// remove constructor implementation
					if ( curDef.contains( ":" ) )
						curDef.remove( curDef.mid( curDef.indexOf( ":" ) ) );
					// get return type.
					rt = curDef.mid( 0, curDef.lastIndexOf( e.getName() ) ).simplified();
					// remove return type.
					curDef = curDef.mid( curDef.lastIndexOf( e.getName() ) );
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
					s = curDef;
				}
				else if ( c == "d" )
				{
					if ( !winMode || ( !e.getName().endsWith( "A" ) && !e.getName().endsWith( "W" ) ) )
						s = e.getName();
				}
				else
					s = e.getName();
				
				// prepend context if available
				if ( !e.getFieldValue( "class" ).isEmpty() )
					s.prepend( e.getFieldValue( "class" ).append( "::" ) );
				else if ( !e.getFieldValue( "struct" ).isEmpty() )
					s.prepend( e.getFieldValue( "struct" ).append( "::" ) );
				else if ( !e.getFieldValue( "namespace" ).isEmpty() )
					s.prepend( e.getFieldValue( "namespace" ).append( "::" ) );
				else if ( !e.getFieldValue( "enum" ).isEmpty() )
					s.prepend( e.getFieldValue( "enum" ).append( "::" ) );
				
				// check return type
				if ( !rt.isEmpty() )
				{
					if ( !e.getFieldValue( "class" ).isEmpty() )
						rt.remove( QRegExp( e.getFieldValue( "class" ).append( "(<.*>)?::" ) ) );
					else if ( !e.getFieldValue( "struct" ).isEmpty() )
						rt.remove( QRegExp( e.getFieldValue( "struct" ).append( "(<.*>)?::" ) ) );
					else if ( !e.getFieldValue( "namespace" ).isEmpty() )
						rt.remove( QRegExp( e.getFieldValue( "namespace" ).append( "(<.*>)?::" ) ) );
					else if ( !e.getFieldValue( "enum" ).isEmpty() )
						rt.remove( QRegExp( e.getFieldValue( "enum" ).append( "(<.*>)?::" ) ) );
					rt = rt.trimmed();
					if ( !rt.isEmpty() )
						s.append( " " +rt );
				}
				
				// append to buffer if needed
				if ( !s.isEmpty() && !lb.contains( qPrintable( s ) ) )
					lb << qPrintable( s );
			}
		}
		
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
	s = getSaveFileName( tr( "Save api file" ), QString::null, tr( "Api Files (*.api)" ), this );
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

bool UICtags2Api::processCtags( const QString& )
{
	return false;
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
