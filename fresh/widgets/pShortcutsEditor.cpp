/********************************************************************************************************
 * PROGRAM      : fresh
 * DATE - TIME  : 2007/06/17
 * AUTHOR       : Nox PasNox ( pasnox@gmail.com )
 * FILENAME     : pshortcutsEditor.cpp
 * LICENSE      : GPL
 * COMMENTARY   : This class allow to edit pAction shortcuts
 ********************************************************************************************************/
#include "pShortcutsEditor.h"
#include "pKeySequenceInput.h"
#include "pActionManager.h"
#include "pAction.h"

#include <QBoxLayout>
#include <QTreeWidget>
#include <QHeaderView>
#include <QPushButton>
#include <QMessageBox>
#include <QSettings>

pShortcutsEditor::pShortcutsEditor( QWidget* w )
	: QDialog( w )
{
	setAttribute( Qt::WA_DeleteOnClose );
	setWindowTitle( tr( "Shortcuts Editor" ) );
	resize( 400, 480 );

	twShortcuts = new QTreeWidget( this );
	twShortcuts->setObjectName( "twShortcuts" );
	twShortcuts->setColumnCount( 2 );
	twShortcuts->header()->setResizeMode( QHeaderView::ResizeToContents );
	twShortcuts->header()->hide();

	pbRestore = new QPushButton( tr( "Restore default" ), this );
	pbRestore->setObjectName( "pbRestore" );
	pbRestore->setEnabled( false );

	pbClear = new QPushButton( tr( "Clear" ), this );
	pbClear->setObjectName( "pbClear" );
	pbClear->setEnabled( false );

	leShortcut = new pKeySequenceInput( this );
	leShortcut->setObjectName( "leShortcut" );
	leShortcut->setEnabled( false );

	pbSet = new QPushButton( tr( "Set" ), this );
	pbSet->setObjectName( "pbSet" );
	pbSet->setEnabled( false );

	pbClose = new QPushButton( tr( "Close" ), this );
	pbClose->setObjectName( "pbClose" );

	QBoxLayout* hl = new QBoxLayout( QBoxLayout::LeftToRight );
	hl->setMargin( 0 );
	hl->setSpacing( 3 );
	hl->addWidget( pbRestore );
	hl->addWidget( pbClear );
	hl->addWidget( leShortcut );
	hl->addWidget( pbSet );

	QBoxLayout* vl = new QBoxLayout( QBoxLayout::TopToBottom, this );
	vl->setMargin( 5 );
	vl->setSpacing( 3 );
	vl->addWidget( twShortcuts );
	vl->addLayout( hl );
	vl->addWidget( pbClose );

	// fill tree
	foreach ( pActionList al, pActionManager::instance()->mActions )
	{
		// group item
		QTreeWidgetItem* gi = 0;
		if ( al.count() )
		{
			QString g = al.at( 0 )->property( pAction::_GROUP_PROPERTY_ ).toString();
			int c = g.split( '/' ).count();

			// create sub group items
			for ( int i = 0; i < c; i++ )
			{
				QString s = g.section( '/', 0, i );
				QTreeWidgetItem* ci = mItems.value( s );
				if ( !ci )
				{
					if ( i == 0 )
						gi = new QTreeWidgetItem( twShortcuts );
					else
						gi = new QTreeWidgetItem( gi );
					mItems[s] = gi;
					gi->setText( 0, g.section( '/', i, i ) );
					gi->setExpanded( true );
				}
				else
					gi = ci;
			}
		}

		foreach ( QAction* a, al )
		{
			// action item
			QTreeWidgetItem* it = new QTreeWidgetItem( gi );
			it->setText( 0, pAction::fixedString( a->text() ) );
			it->setText( 1, a->shortcut().toString() );
			it->setIcon( 0, a->icon() );
			// stock pointer action in data
			it->setData( 0, Qt::UserRole, reinterpret_cast<quintptr>( a ) );
		}
	}

	// connections
	connect( pbClear, SIGNAL( clicked() ), leShortcut, SLOT( clear() ) );
	connect( pbRestore, SIGNAL( clicked() ), this, SLOT( pbRestore_pbSet_clicked() ) );
	connect( pbSet, SIGNAL( clicked() ), this, SLOT( pbRestore_pbSet_clicked() ) );
	connect( pbClose, SIGNAL( clicked() ), this, SLOT( close() ) );
	QMetaObject::connectSlotsByName( this );
}

void pShortcutsEditor::on_twShortcuts_itemSelectionChanged()
{
	// get selected item
	QTreeWidgetItem* it = twShortcuts->selectedItems().value( 0 );
	// get action
	QAction* a = it ? reinterpret_cast<QAction*>( it->data( 0, Qt::UserRole ).toULongLong() ) : 0;

	// set button state according to item is an action
	pbRestore->setEnabled( a );
	pbClear->setEnabled( a );
	leShortcut->setEnabled( a );
	leShortcut->clear();
	pbSet->setEnabled( a );

	// return if no action
	if ( !a )
		return;

	// set shortcut
	leShortcut->setText( a->shortcut().toString() );

	// give focus to lineedit
	leShortcut->setFocus();
}

void pShortcutsEditor::pbRestore_pbSet_clicked()
{
	// get selected item
	QTreeWidgetItem* it = twShortcuts->selectedItems().value( 0 );

	// get action
	pAction* a = reinterpret_cast<pAction*>( it->data( 0, Qt::UserRole ).toULongLong() );

	if ( a )
	{
		// get default action shortcut
		QString s = sender() == pbRestore ? a->property( pAction::_DEFAULT_SHORTCUT_PROPERTY_ ).toString() : leShortcut->text();

		// try asigning new shortcut
		if ( pActionManager::instance()->setShortcut( a, QKeySequence( s ) ) )
		{
			it->setText( 1, s );
			leShortcut->setText( s );
			pbSet->setEnabled( false );
		}
		// show warning
		else
		{
			QMessageBox::warning( window(), tr( "Error" ), pActionManager::lastError(), QMessageBox::Close );
			leShortcut->setText( a->shortcut().toString() );
		}
	}
}

void pShortcutsEditor::on_leShortcut_textChanged( const QString& )
{
	pbSet->setEnabled( true );
}
