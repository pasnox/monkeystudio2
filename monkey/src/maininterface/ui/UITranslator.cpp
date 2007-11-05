/****************************************************************************
**
** 		Created using Monkey Studio v1.8.0.0
** Author    : Azevedo Filipe aka Nox P@sNox <pasnox@gmail.com>, The Monkey Studio Team
** Project   : Monkey Studio 2
** FileName  : UITranslator.cpp
** Date      : 2007-11-04T22:45:11
** License   : GPL
** Comment   : Monkey Studio is a Free, Fast and lightweight crossplatform Qt RAD.
It's extendable with a powerfull plugins system.
** Home Page : http://www.monkeystudio.org
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/
#include "UITranslator.h"
#include "pSettings.h"

#include <QLibraryInfo>
#include <QFileDialog>
#include <QLocale>

UITranslator::UITranslator( QWidget* p )
	: QDialog( p )
{
	setupUi( this );
	mTranslationsPath = pSettings::instance()->value( "Paths/Translations", QLibraryInfo::location( QLibraryInfo::TranslationsPath ) ).toString();
	on_tbReload_clicked();
}

void UITranslator::accept()
{
	if ( lwTranslations->currentItem() )
	{
		pSettings::instance()->setValue( "Language", lwTranslations->currentItem()->text() );
		pSettings::instance()->setValue( "Paths/Translations", mTranslationsPath );
		QDialog::accept();
	}
}

void UITranslator::on_tbLocate_clicked()
{
	QString s = QFileDialog::getExistingDirectory( this, tr( "Choose the location of your translations" ), mTranslationsPath );
	if ( !s.isEmpty() )
	{
		mTranslationsPath = s;
		on_tbReload_clicked();
	}
}

void UITranslator::on_tbReload_clicked()
{
	lwTranslations->clear();
	QDir d( mTranslationsPath );
	if ( !d.exists() )
		return;
	QStringList l = d.entryList( QStringList() << "monkey_*.qm", QDir::Files | QDir::Readable );
	lwTranslations->addItem( "english" );
	foreach ( QString s, l )
		lwTranslations->addItem( s.remove( "monkey_" ).remove( ".qm" ) );
	QList<QListWidgetItem*> li = lwTranslations->findItems( QLocale::languageToString( QLocale::system().language() ), Qt::MatchFixedString );
	if ( li.count() )
		lwTranslations->setCurrentItem( li.at( 0 ) );
}
