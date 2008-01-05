/********************************************************************************************************
 * PROGRAM      : fresh
 * DATE - TIME  : 2007/06/17
 * AUTHOR       : Nox PasNox ( pasnox@gmail.com )
 * FILENAME     : pExtendedWorkspace.cpp
 * LICENSE      : GPL
 * COMMENTARY   : This class provide a workspace that can at run time be switched in SDI / MDI or Top Level
 ********************************************************************************************************/
#include "pExtendedWorkspace.h"
#include "pFilesListWidget.h"

#include <QBoxLayout>
#include <QMdiArea>
#include <QMdiSubWindow>
#include <QIcon>
#include <QCloseEvent>
#include <QFile>
#include <QMenu>
#include <QMainWindow>
#include <QStackedLayout>
#include <QStackedWidget>
#include <QDockWidget>
#include <Qt>
#include <QApplication>

pExtendedWorkspace::pExtendedWorkspace( QWidget* w, pExtendedWorkspace::DocumentMode m )
	: QWidget( w )
{
	// tab widget
	mTabLayout = new QBoxLayout( QBoxLayout::LeftToRight );
	mTabLayout->setSpacing( 3 );
	mTabLayout->setMargin( 0 );
	//mTabBar = new pTabBar (this);
	//mTabLayout->addWidget( ( mTabBar ) );

	mFilesList = new pFilesListWidget( tr( "Files List"), this );
	
	// document widget
	mStackedLayout = new QStackedLayout;
	mStackedLayout->addWidget( ( mStackedWidget = new QStackedWidget ) );
	mStackedLayout->addWidget( ( mMdiAreaWidget = new QMdiArea ) );

	// main layout
	mLayout = new QBoxLayout( QBoxLayout::TopToBottom, this );
	mLayout->setSpacing( 0 );
	mLayout->setMargin( 0 );
	mLayout->addLayout( mTabLayout );
    mLayout->addLayout( mStackedLayout );
	
	// init view
	//mTabBar->setDrawBase( false );
	//mTabBar->setSizePolicy( QSizePolicy( QSizePolicy::Expanding, QSizePolicy::Preferred ) );
	
	mDocMode = (DocumentMode) -1; //for avoid return on start method setDocMode (m)
	setDocMode( m );
	
	connect( mMdiAreaWidget, SIGNAL( subWindowActivated( QMdiSubWindow* ) ), this, SLOT( setCurrentDocument( QMdiSubWindow* ) ) );
}

pExtendedWorkspace::~pExtendedWorkspace()
{
	// close all document
	closeAllDocuments();

	// delete container
	delete mMdiAreaWidget;
}

bool pExtendedWorkspace::eventFilter( QObject* o, QEvent* e )
{
	// get document
	QWidget* td = qobject_cast<QWidget*>( o );
	
	if ( indexOf( td ) == -1 )
		return QWidget::eventFilter( o, e );
	
	// get event type
	QEvent::Type t = e->type();
	
	// child modified state
	switch ( t )
	{
	case QEvent::ModifiedChange:
		emit modifiedChanged( indexOf( td ), td->isWindowModified() );
		break;
	case QEvent::Close:
		closeDocument( td );
		return true;
		break;
	case QEvent::WindowActivate:
		if ( mDocMode == dmTopLevel )
			setCurrentDocument( td );
		break;
	case QEvent::WindowTitleChange:
		emit docTitleChanged( indexOf( td ), td->windowTitle().replace( "[*]", QString() ) );
	default:
		break;
	}

	// return default event filter
	return QWidget::eventFilter( o, e );
}

pTabBar* pExtendedWorkspace::tabBar() const
{ return NULL;}//mTabBar; }

pFilesListWidget* pExtendedWorkspace::listWidget() const
{ return mFilesList; }

pExtendedWorkspace::DocumentMode pExtendedWorkspace::docMode() const
{ return mDocMode; }

QTabBar::Shape pExtendedWorkspace::tabShape () const
{ return (QTabBar::Shape)0;}//mTabBar->shape(); }

QWidgetList pExtendedWorkspace::documents() const
{ return mDocuments; }

QWidget* pExtendedWorkspace::document( int i ) const
{ return mDocuments.value( i ); }

int pExtendedWorkspace::count() const
{ return mDocuments.count(); }

int pExtendedWorkspace::currentIndex() const
{ return indexOf( currentDocument() ); }

QWidget* pExtendedWorkspace::currentDocument() const
{
	switch ( mDocMode )
	{
		case dmSDI:
			return mStackedWidget->currentWidget();
			break;
		case dmMDI:
			return mMdiAreaWidget->currentSubWindow() ? mMdiAreaWidget->currentSubWindow()->widget() : 0;
			break;
		case dmTopLevel:
			foreach ( QWidget* w, mDocuments )
				if ( qApp->activeWindow() == w )
					return w;
			break;
	}
	return 0;
}

int pExtendedWorkspace::indexOf(QWidget* w) const
{ return mDocuments.indexOf (w); }

void pExtendedWorkspace::setBackground( const QPixmap& p )
{ mMdiAreaWidget->setBackground( QBrush( p ) ); }

void pExtendedWorkspace::setBackground( const QString& s )
{ mMdiAreaWidget->setBackground( QBrush( QPixmap( s ) ) ); }

int pExtendedWorkspace::addDocument(QWidget* td, const QString& s,  const QIcon& i)
{
	return insertDocument (count(), td, s, i);
}

int pExtendedWorkspace::insertDocument(int pos, QWidget* td, const QString&,  const QIcon& i)
{
	// filter the document
	td->installEventFilter( this );
	td->setAttribute (Qt::WA_DeleteOnClose, true);

	// append to document list
	mDocuments.insert( pos, td );
	
	switch ( mDocMode )
	{
	case dmSDI:
		mStackedLayout->setCurrentWidget( mStackedWidget );
		mStackedWidget->addWidget( td );
		//mStackedWidget->setCurrentWidget (td);
		break;
	case dmMDI:
		mMdiAreaWidget->addSubWindow( td )->showNormal();
		break;
	case dmTopLevel:
		td->setParent( 0 );
		//td->addActions (mMainWindow->actions());//not working !!!! FIXME
		td->showNormal();
		break;
	}	
	
	// emit tab inserted
	emit documentInserted( pos, /*s*/td->windowTitle ().replace("[*]", ""), i );

	// emit tab current changed
	emit currentChanged( pos );
	
	return pos;
}

void pExtendedWorkspace::closeDocument(int i)
{

	//signal must be processed while widget is exists for avoid crashs
	emit documentAboutToClose( i );
	
	mDocuments[i]->removeEventFilter( this );
	if (mDocMode == dmMDI)
		foreach (QMdiSubWindow* sw, mMdiAreaWidget->subWindowList ())
			if (sw->widget () == mDocuments[i])
				delete (sw);
	mDocuments[i]->setParent (NULL);
	mDocuments[i]->close ();
	
	// remove document
	mDocuments.removeAt(i);
	
			/*
	if (i == currentIndex() && count() > 0 )
	{
        setCurrentIndex (0);
	}
	else if ( count() == 0 ) //last was closed 
			*/
	emit currentChanged( currentIndex() );
}

void pExtendedWorkspace::closeDocument( QWidget* td )
{
	return closeDocument (indexOf (td));
}

void pExtendedWorkspace::closeAllDocuments()
{
	for (int i = count()-1; i>=0; i--)
		closeDocument (i);
}

void pExtendedWorkspace::closeCurrentDocument()
{
	closeDocument (currentIndex());
}

void pExtendedWorkspace::setDocMode( pExtendedWorkspace::DocumentMode dm )
{
	if ( mDocMode == dm )
		return;
	
	mDocMode = dm;

    if (mDocMode == dmSDI)
        mStackedLayout->setCurrentWidget( mStackedWidget );
    else if (mDocMode ==dmMDI )
		mStackedLayout->setCurrentWidget( mMdiAreaWidget );

	if (!count())
		return;
	
	int i = currentIndex(); //for avoid return from function because index not changed

	// add document to correct workspace
	foreach ( QWidget* td, mDocuments )
	{
		switch ( mDocMode )
		{
		case dmSDI:
            mStackedWidget->addWidget( td );
			//foreach (QAction* act, mMainWindow->actions ()) // not working !!! FIXME
			//	td->removeAction (act);
			break;
		case dmMDI:
			mMdiAreaWidget->addSubWindow( td )->widget()->showNormal();
			//foreach (QAction* act, mMainWindow->actions ())
			//	td->removeAction (act);
			break;
		case dmTopLevel:
			td->setParent( 0 );
			//td->addActions (mMainWindow->actions());
			td->showNormal();
			break;
		}	
	}
	
	//cleanup QMdiArea - remove it's empty QMdiSubWindows
	if ( mDocMode != dmMDI )
		foreach (QMdiSubWindow* sw, mMdiAreaWidget->subWindowList ())
			delete  sw; //if just remove - widget will not be deleted
	
	// restore current index
	setCurrentIndex( i );
    
	// emit tab mode changed
	emit docModeChanged( mDocMode );
}

void pExtendedWorkspace::setTabShape( QTabBar::Shape )
{
/*	if ( tabShape() == s )
		return;
	
	// get sizepolicy
	QSizePolicy sp = mTabBar->sizePolicy();
	
	// update view layout
	switch ( s )
	{
	case QTabBar::RoundedNorth:
	case QTabBar::TriangularNorth:
		mLayout->setDirection( QBoxLayout::TopToBottom );
		break;
	case QTabBar::RoundedSouth:
	case QTabBar::TriangularSouth:
		mLayout->setDirection( QBoxLayout::BottomToTop );
		break;
	default:
		Q_ASSERT (0);
	}
	
	// apply tab shape
	mTabBar->setShape( s );
	
	// update corners
	//updateCorners();
	
	// emit shape changed
	emit tabShapeChanged( s );*/
}

void pExtendedWorkspace::setCurrentIndex( int i )
{
	if ( currentIndex() == i || i < 0)
		return;
	
	switch ( mDocMode )
	{
		case dmSDI:
            mStackedWidget->setCurrentWidget (document( i ));
		case dmMDI:
			mMdiAreaWidget->setActiveSubWindow( qobject_cast<QMdiSubWindow*>(document( i )->parent() ));
			break;
		case dmTopLevel:
			if (!document( i )->isActiveWindow ())
				document( i )->activateWindow();
			break;
	}
	// emit document change
	emit currentChanged( i );
}

void pExtendedWorkspace::setCurrentDocument( QWidget* d )
{ setCurrentIndex( indexOf( d ) ); }

void pExtendedWorkspace::setCurrentDocument( QMdiSubWindow* d )
{ 
	if (d)
		setCurrentIndex( indexOf( d->widget() ) ); 
}

void pExtendedWorkspace::activateNextDocument()
{
	int currIndex = currentIndex();
	if ( currIndex +1 == count() )
		setCurrentIndex( 0 );
	else
		setCurrentIndex( currIndex +1 );
}

void pExtendedWorkspace::activatePreviousDocument()
{
	int currIndex = currentIndex();
	if ( currIndex -1 == -1 )
		setCurrentIndex( count() -1 );
	else
		setCurrentIndex( currIndex -1 );
}

void pExtendedWorkspace::setSDI ()
{
    setDocMode (dmSDI);
}

void pExtendedWorkspace::setMDI ()
{
    setDocMode (dmMDI);
}

void pExtendedWorkspace::setTopLevel ()
{
    setDocMode (dmTopLevel);
}

void pExtendedWorkspace::cascade ()
{
    setDocMode (dmMDI);
    mMdiAreaWidget->cascadeSubWindows ();
};

void pExtendedWorkspace::tile ()
{
    setDocMode (dmMDI);
    mMdiAreaWidget->tileSubWindows ();
};

void pExtendedWorkspace::minimize ()
{
    foreach (QWidget* w, mDocuments)
        w->showMinimized ();  //WTF ???
};

void pExtendedWorkspace::restore ()
{
    foreach (QWidget* w, mDocuments)
        w->showNormal();
};
