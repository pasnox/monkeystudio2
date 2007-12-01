#include "pFileListEditor.h"

#include <QToolBar>
#include <QAction>
#include <QFileDialog>
#include <QListWidgetItem>

pFileListEditor::pFileListEditor( QWidget* p, const QString& t, const QString& pa, const QString& f ):
	pStringListEditor( p, t ), mPath( pa ), mFilter( f )
{ findChild<QToolBar*>()->actions().last()->setIcon( QIcon( ":/stringlisteditor/icons/stringlisteditor/file.png" ) ); }

void pFileListEditor::onAddItem()
{
	// get files
	QStringList l = QFileDialog::getOpenFileNames( window(), tr( "Choose file(s)" ), mPath, mFilter );
	
	if ( !l.isEmpty() )
	{
		foreach ( QString s, l )
		{
			QListWidgetItem* it = new QListWidgetItem( s, mList );
			it->setFlags( it->flags() | Qt::ItemIsEditable );
			mList->setCurrentItem( it );
			mList->scrollToItem( it );
		}
		emit edited();
	}
}

void pFileListEditor::onEditItem()
{
	if ( QListWidgetItem* it = mList->selectedItems().value( 0 ) )
	{
		QString s= QFileDialog::getOpenFileName( window(), tr( "Choose file" ), mPath, mFilter );
		if ( !s.isEmpty() )
		{
			it->setText( s );
			emit edited();
		}
	}
}

void pFileListEditor::setPath( const QString& s )
{ mPath = s; }
