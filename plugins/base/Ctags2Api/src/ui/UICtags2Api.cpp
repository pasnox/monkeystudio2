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

bool UICtags2Api::processCtagsBuffer( const QByteArray& a )
{
	if ( a.isEmpty() )
		return false;
	
	QBuffer b;
	b.setData( a );
	if ( !b.open( QBuffer::ReadOnly | QFile::Text ) )
		return false;
	
	// output buffer
	QByteArray ob;
	
	// set progress bar
	pbLoading->setVisible( true );
	pbLoading->setValue( 0 );
	pbLoading->setMaximum( a.split( '\n' ).count() -1 );
	
	//
	/*
	QStringList contents;
	QHash<QString, QStringList> mCache;
	*/
	// process buffer
	QString s, c;
	bool removePrivate = true;
	bool winMode = true;
	QString include = "A";
	int curLineNo;
	
	while ( b.canReadLine() )
	{
		// get line
		const QString l = QTextCodec::codecForLocale()->toUnicode( b.readLine() );
		/*
		// if no comment line
		if ( l[0] != '!' )
		{
			// create ctags entity
			CtagsEntity e( l );
			curLineNo = e.getAddress().toInt() -1; // ctags file start at 1
			c = e.getFile().prepend( "/home/pasnox/Desktop/qt/src/" ).replace( "\\", "/" );
			if ( !mCache.keys().contains( c ) )
			{
				QFile f( c );
				if ( !f.open( QFile::ReadOnly | QFile::Text ) )
				{
					//warning( tr( "Open File..." ), tr( "Can't open file :\n%1" ).arg( e.getFile().prepend( "/home/pasnox/Desktop/qt/src/" ).replace( "\\", "/" ) ) );
					continue;
				}
				mCache[c] = QTextCodec::codecForLocale()->toUnicode( f.readAll() ).split( '\n' );
			}
			contents = mCache[c];
			
			if ( ( !removePrivate || e.getName()[0] != '_' ) && !e.getName().startsWith( "operator " ) )
			{
				c = e.getKindValue();
				if ( c == "p" || s == "f" )
				{
					
					try:
					braces = bracesDiff(contents[curLineNo])
					curDef = contents[curLineNo]
					while braces > 0:	# Search for end of prototype.
						curLineNo = curLineNo + 1
						braces = braces + bracesDiff(contents[curLineNo])
						curDef = curDef + contents[curLineNo]
					# Normalise the appearance of the prototype.
					curDef = string.strip(curDef)
					# Replace whitespace sequences with a single space character.
					curDef = string.join(string.split(curDef))
					# Remove space around the '('.
					curDef = string.replace(string.replace(curDef, " (", '('), "( ", '(')
					# Remove trailing semicolon.
					curDef = string.replace(curDef, ";", '')
					# Remove implementation if present.
					if "{" in curDef and "}" in curDef:
						startImpl = curDef.find("{")
						endImpl = curDef.find("}")
						curDef = curDef[:startImpl] + curDef[endImpl+1:]
					else:
						# Remove trailing brace.
						curDef = curDef.rstrip("{")
					# Remove return type.
					curDef = curDef[string.find(curDef, entityName):]
					# Remove virtual indicator.
					if curDef.replace(" ", "").endswith(")=0"):
						curDef = curDef.rstrip("0 ")
						curDef = curDef.rstrip("= ")
					# Remove trailing space.
					curDef = curDef.rstrip()
					if winMode:
						if string.find(curDef, "A(") >= 0:
							if "A" in include:
								apis.add(string.replace(curDef, "A(", '('))
						elif string.find(curDef, "W(") >= 0:
							if "W" in include:
								apis.add(string.replace(curDef, "W(", '('))
						else:	# A character set independent function.
							apis.add(curDef)
					else:
						apis.add(curDef)
				except IndexError:
					pass
					
				}
				else if ( c == "d" )
				{
					QString curDef = contents[curLineNo];
					if ( !winMode || ( !curDef.endsWith( "A" ) && !curDef.endsWith( "W" ) ) )
						s = e.getName();
				}
				else
					s = e.getName();
			}
		}
		*/
		
		
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
					s = e.getAddress();
					s.replace( "/^", "" ).replace( "$/", "" ).replace( ";\"", "" );
				}
				else if ( c == "d" )
				{
					if ( !winMode || ( !e.getName().endsWith( "A" ) && !e.getName().endsWith( "W" ) ) )
						s = e.getName();
				}
				else
					s = e.getName();
			}
		}
		/*
		// check class / struct
		if ( !( c = e.getFieldValue( "class" ) ).isEmpty() )
			s = QString( "%1::%2" ).arg( c ).arg( e.getName() );
		else if ( !( c = e.getFieldValue( "struct" ) ).isEmpty() )
			s = QString( "%1::%2" ).arg( c ).arg( e.getName() );
		*/
		// append to buffer
		if ( !s.isEmpty() )
			ob.append( qPrintable( s +"\n" ) );
		
		// increase progressbar
		pbLoading->setValue( pbLoading->value() +1 );
		QApplication::processEvents();
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
		f.write( ob );
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
