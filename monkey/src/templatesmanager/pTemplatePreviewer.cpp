/****************************************************************************
**
** 		Created using Monkey Studio v1.8.0.0
** Author    : Azevedo Filipe aka Nox P@sNox <pasnox@gmail.com>, The Monkey Studio Team
** Project   : Monkey Studio 2
** FileName  : pTemplatePreviewer.cpp
** Date      : 2007-11-04T22:46:26
** License   : GPL
** Comment   : Monkey Studio is a Free, Fast and lightweight crossplatform Qt RAD.
It's extendable with a powerfull plugins system.
** Home Page : http://www.monkeystudio.org
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/
#include "pTemplatePreviewer.h"
#include "pEditor.h"

#include <QVariant>
#include <QLineEdit>
#include <QVBoxLayout>

pTemplatePreviewer::pTemplatePreviewer( QWidget* w )
	: QWidget( w )
{
	QVBoxLayout* mLayout = new QVBoxLayout( this );
	mLayout->setMargin( 0 );
	mLayout->setSpacing( 3 );
	mLineEdit = new QLineEdit;
	mEditor = new pEditor;
	mLayout->addWidget( mLineEdit );
	mLayout->addWidget( mEditor );
}

pTemplatePreviewer::~pTemplatePreviewer()
{
}

void pTemplatePreviewer::setFileName( const QString& s )
{
	mLineEdit->setText( s );
}

QString pTemplatePreviewer::fileName() const
{
	return mLineEdit->text();
}

void pTemplatePreviewer::setDestination( const QString& s )
{
	setProperty( "pdestination", s );
}

QString pTemplatePreviewer::destination() const
{
	return property( "pdestination" ).toString();
}

void pTemplatePreviewer::setOpen( bool b )
{
	setProperty( "pOpen", b );
}

bool pTemplatePreviewer::open() const
{
	return property( "pOpen" ).toBool();
}

bool pTemplatePreviewer::open( const QString& s )
{
	setOpen( mEditor->openFile( s ) );
	return open();
}

pEditor* pTemplatePreviewer::editor() const
{
	return mEditor;
}
