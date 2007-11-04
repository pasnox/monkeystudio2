/****************************************************************************
**
** 		Created using Monkey Studio v1.8.0.0
** Author    : Azevedo Filipe aka Nox P@sNox <pasnox@gmail.com>, The Monkey Studio Team
** Project   : Monkey Studio 2
** FileName  : QMakeItemDelegate.h
** Date      : 2007-11-04T22:53:42
** License   : GPL
** Comment   : Monkey Studio is a Free, Fast and lightweight crossplatform Qt RAD.
It's extendable with a powerfull plugins system.
** Home Page : http://www.monkeystudio.org
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/#ifndef QMAKEITEMDELEGATE_H
#define QMAKEITEMDELEGATE_H

#include <QItemDelegate>
#include <QHBoxLayout>
#include <QComboBox>
#include <QLineEdit>
#include <QToolButton>
#include <QEvent>
#include <QCursor>
#include <QFocusEvent>
#include <QApplication>

#include "UIItemSettings.h"
#include "ProjectItem.h"
#include "pMonkeyStudio.h"

using namespace pMonkeyStudio;

class QMakeItemDelegate : public QItemDelegate
{
	Q_OBJECT
	
public:
	QMakeItemDelegate( QWidget* );
	virtual QWidget* createEditor( QWidget* , const QStyleOptionViewItem&, const QModelIndex& ) const;
	virtual void setModelData( QWidget*, QAbstractItemModel* m, const QModelIndex& ) const;
	
};

class QValueFileFolderEdit : public QWidget
{
	Q_OBJECT
	
private:
	QComboBox* cbValue;
	QToolButton* tbFile;
	QToolButton* tbFolder;
	
public:
	QValueFileFolderEdit( QWidget* p, const QModelIndex& i )
		: QWidget( p )
	{
		QHBoxLayout* hb = new QHBoxLayout( this );
		hb->setMargin( 0 );
		hb->setSpacing( 1 );
		hb->addWidget( ( cbValue = new QComboBox( this ) ), 100 );
		hb->addWidget( ( tbFile = new QToolButton( this ) ) );
		hb->addWidget( ( tbFolder = new QToolButton( this ) ) );
		tbFile->setIcon( QPixmap( ":/icons/icons/filefolder.png" ) );
		tbFolder->setIcon( QPixmap( ":/icons/icons/folder.png" ) );
		cbValue->setEditable( true );
		setText( i.data().toString() );
		
		setFocusProxy( cbValue );
		setFocusPolicy( Qt::ClickFocus );
		cbValue->installEventFilter( this );
		
		connect( tbFile, SIGNAL( clicked() ), this, SLOT( tbClicked() ) );
		connect( tbFolder, SIGNAL( clicked() ), this, SLOT( tbClicked() ) );
	}
	
	bool eventFilter( QObject* o, QEvent* e )
	{
		if ( e->type() == QEvent::FocusOut )
			if ( !isAncestorOf( QApplication::widgetAt( QCursor::pos() ) ) )
				QApplication::sendEvent( this, e );
		return QWidget::eventFilter( o, e );
	}
	
	void setText( const QString& s )
	{
		if ( cbValue->findText( s ) == -1 )
			cbValue->addItem( s );
		cbValue->setEditText( s );
	}
	
	QString text() const
	{ return cbValue->currentText(); }
	
protected slots:
	void tbClicked()
	{
		QToolButton* tb = qobject_cast<QToolButton*>( sender() );
		if ( !tb )
			return;
		ProjectItem* it = UIItemSettings::instance( (ProjectItem*)0 )->mItem;
		if ( !it )
			return;
		QString s;
		if ( tb == tbFile )
			s = getOpenFileName( QString::null, it->canonicalFilePath( text() ), QString::null, this );
		else
			s = getExistingDirectory( QString::null, it->canonicalFilePath( text() ), this );
		if ( !s.isNull() )
			setText( it->relativeFilePath( s ) );
	}
	
};

#endif // QMAKEITEMDELEGATE_H
