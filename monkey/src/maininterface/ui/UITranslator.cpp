#include "UITranslator.h"
#include "MonkeyCore.h"
#include "pSettings.h"
#include "pMonkeyStudio.h"

#include <QLibraryInfo>
#include <QFileDialog>
#include <QLocale>

UITranslator::UITranslator( QWidget* p )
	: QDialog( p )
{
	setupUi( this );
	setAttribute( Qt::WA_DeleteOnClose );
	mTranslationsPath = MonkeyCore::settings()->value( "Translations/Path", QLibraryInfo::location( QLibraryInfo::TranslationsPath ) ).toString();
	on_tbReload_clicked();
}

void UITranslator::accept()
{
	if ( lwTranslations->currentItem() )
	{
		pSettings* s = MonkeyCore::settings();
		s->setValue( "Translations/Language", lwTranslations->currentItem()->text() );
		s->setValue( "Translations/Accepted", true );
		s->setValue( "Translations/Path", mTranslationsPath );
		if ( parentWidget() )
			pMonkeyStudio::warning( tr( "Changing Translations..." ), tr( "You need to restart %1 for the new translations be applied" ).arg( PROGRAM_NAME ), window() );
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
