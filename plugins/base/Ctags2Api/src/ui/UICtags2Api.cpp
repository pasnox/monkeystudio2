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
			s = getExistingDirectory( tr( "Select the include path to scan" ), leIncludeDirectory->text(), this );
			if ( !s.isNull() )
				leIncludeDirectory->setText( s );
			break;
		case 1: // api
			s = getOpenFileName( tr( "Select the api file to prepare" ), leApiFile->text(), QString::null, this );
			if ( !s.isNull() )
				leApiFile->setText( s );
			break;
		case 2: // ctags
			s = getOpenFileName( tr( "Select the tags file to convert" ), leCtagsFile->text(), QString::null, this );
			if ( !s.isNull() )
				leCtagsFile->setText( s );
	}
}
