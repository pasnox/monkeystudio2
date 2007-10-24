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

using namespace pMonkeyStudio;

UICtags2Api::UICtags2Api( QWidget* w )
	: QDialog( w )
{ setupUi( this ); }

UICtags2Api::~UICtags2Api()
{
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

bool UICtags2Api::processCtagsBuffer( const QByteArray& a )
{
	if ( a.isEmpty() )
		return false;
	
	QBuffer b;
	b.setData( a );
	if ( !b.open( QBuffer::ReadOnly ) )
		return false;
	
	// output buffer
	QByteArray ob;
	
	while ( b.canReadLine() )
	{
		CtagsEntity e( QTextCodec::codecForLocale()->toUnicode( b.readLine() ) );
		QString s, c;
		
		// check class / struct
		if ( !( c = e.getFieldValue( "class" ) ).isEmpty() )
			s = QString( "%1::%2" ).arg( c ).arg( e.getName() );
		else if ( !( c = e.getFieldValue( "struct" ) ).isEmpty() )
			s = QString( "%1::%2" ).arg( c ).arg( e.getName() );
		
		// append to buffer
		if ( !s.isEmpty() )
			ob.append( qPrintable( s +"\n" ) );
	}
	
	warning( "", ob );
	
	return true;
}

bool UICtags2Api::processCtags( const QString& s )
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
