/****************************************************************************
**
** 		Created using Monkey Studio v1.8.1.0
** Authors    : Filipe AZEVEDO aka Nox P@sNox <pasnox@gmail.com>
** Project   : Monkey Studio IDE
** FileName  : UITranslator.cpp
** Date      : 2008-01-14T00:36:54
** License   : GPL
** Comment   : This header has been automatically generated, if you are the original author, or co-author, fill free to replace/append with your informations.
** Home Page : http://www.monkeystudio.org
**
    Copyright (C) 2005 - 2008  Filipe AZEVEDO & The Monkey Studio Team

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
**
****************************************************************************/
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
	mTranslationsPath = MonkeyCore::settings()->value( "Translations/Path" ).toStringList();
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
	QString s = QFileDialog::getExistingDirectory( this, tr( "Choose the location of your translations" ), mTranslationsPath.value( 0 ) );
	if ( !s.isEmpty() )
	{
		if ( !mTranslationsPath.contains( s ) )
			mTranslationsPath << s;
		on_tbReload_clicked();
	}
}

void UITranslator::on_tbReload_clicked()
{
	lwTranslations->clear();
	lwTranslations->addItem( "english" );
	QDir d;
	QStringList l;
	foreach ( QString s, mTranslationsPath )
	{
		d.setPath( QDir::isRelativePath( s ) ? qApp->applicationDirPath().append( "/%1" ).arg( s ) : s );
		if ( !d.exists() )
			continue;
		foreach ( QString s, d.entryList( QStringList() << "monkey_*.qm", QDir::Files | QDir::Readable ) )
		{
			if ( !l.contains( s ) )
			{
				l << s;
				lwTranslations->addItem( s.remove( "monkey_" ).remove( ".qm" ) );
			}
		}
	}
	QList<QListWidgetItem*> li = lwTranslations->findItems( QLocale::languageToString( QLocale::system().language() ), Qt::MatchFixedString );
	if ( li.count() )
		lwTranslations->setCurrentItem( li.at( 0 ) );
}
