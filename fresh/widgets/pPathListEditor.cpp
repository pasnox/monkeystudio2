#include "pPathListEditor.h"

#include <QToolBar>
#include <QAction>
#include <QFileDialog>
#include <QListWidgetItem>

pPathListEditor::pPathListEditor( QWidget* p, const QString& t, const QString& pa ):
	pFileListEditor( p, t, pa, QString() )
{ findChild<QToolBar*>()->actions().last()->setIcon( QIcon( ":/listeditor/icons/listeditor/folder.png" ) ); }

void pPathListEditor::onAddItem()
{
	// get directory
	QString s = QFileDialog::getExistingDirectory( window(), tr( "Choose directory" ), mPath );
	
	if ( !s.isEmpty() )
	{
		QListWidgetItem* it = new QListWidgetItem( s, mList );
		it->setFlags( it->flags() | Qt::ItemIsEditable );
		mList->setCurrentItem( it );
		mList->scrollToItem( it );
		emit edited();
	}
}

void pPathListEditor::onEditItem()
{
	if ( QListWidgetItem* it = mList->selectedItems().value( 0 ) )
	{
		QString s= QFileDialog::getExistingDirectory( window(), tr( "Choose directory" ), mPath );
		if ( !s.isEmpty() )
		{
			it->setText( s );
			emit edited();
		}
	}
}
