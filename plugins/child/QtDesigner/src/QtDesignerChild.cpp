/****************************************************************************
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
****************************************************************************/
#include "QtDesignerChild.h"
#include "QtDesignerManager.h"

#include "widgethost.h"

#include <pIconManager.h>
#include <MonkeyCore.h>
#include <QueuedStatusBar.h>

#include <QDesignerFormWindowManagerInterface>
#include <QDesignerFormWindowInterface>
#include <QDesignerFormEditorInterface>
#include <QDesignerPropertyEditorInterface>
#include <QDesignerPropertySheetExtension>
#include <QExtensionManager>

#include <QPainter>
#include <QPrintDialog>

QtDesignerChild::QtDesignerChild( QtDesignerManager* manager )
	: pAbstractChild()
{
	Q_ASSERT( manager );
	mDesignerManager = manager;
	
	// set up ui
	setWindowIcon( pIconManager::icon( "designer.png", ":/icons" ) );
	
	// create form host widget
	QDesignerFormWindowInterface* form = mDesignerManager->createNewForm( this );
	mDesignerManager->addFormWindow( form );
	
	mHostWidget = new SharedTools::WidgetHost( this, form );
	mHostWidget->setFocusPolicy( Qt::NoFocus );
	
	QVBoxLayout* vl = new QVBoxLayout( this );
	vl->setMargin( 0 );
	vl->setSpacing( 0 );
	vl->addWidget( mHostWidget );
	
	connect( mHostWidget->formWindow(), SIGNAL( changed() ), this, SLOT( formChanged() ) );
	connect( mHostWidget->formWindow(), SIGNAL( selectionChanged() ), this, SLOT( formSelectionChanged() ) );
	connect( mHostWidget->formWindow(), SIGNAL( geometryChanged() ), this, SLOT( formGeometryChanged() ) );
	connect( mHostWidget->formWindow(), SIGNAL( mainContainerChanged( QWidget* ) ), this, SLOT( formMainContainerChanged( QWidget* ) ) );
}

QtDesignerChild::~QtDesignerChild()
{
	// close all files
	closeFiles();
}

void QtDesignerChild::showEvent( QShowEvent* event )
{
	pAbstractChild::showEvent( event );
	
	mDesignerManager->setActiveFormWindow( mHostWidget->formWindow() );
}

void QtDesignerChild::formChanged()
{
	setWindowModified( mHostWidget->formWindow()->isDirty() );
	emit modifiedChanged( mHostWidget->formWindow()->isDirty() );
	emit contentChanged();
}

void QtDesignerChild::formSelectionChanged()
{
	mHostWidget->updateFormWindowSelectionHandles( true );
}

void QtDesignerChild::formGeometryChanged()
{
	// update property
	QDesignerPropertySheetExtension* sheet = qt_extension<QDesignerPropertySheetExtension*>( mDesignerManager->core()->extensionManager(), mHostWidget->formWindow() );

	mDesignerManager->core()->propertyEditor()->setPropertyValue( "geometry", sheet->property( sheet->indexOf( "geometry" ) ) );

	// set modified state
	bool loading = property( "loadingFile" ).toBool();
	bool modified = !loading;
	
	mHostWidget->formWindow()->setDirty( modified );
	setWindowModified( modified );
	setProperty( "loadingFile", false );
	
	// emit modified state
	emit modifiedChanged( modified );
	emit contentChanged();
}

void QtDesignerChild::formMainContainerChanged( QWidget* widget )
{
	Q_UNUSED( widget );
	setProperty( "loadingFile", true );
}

bool QtDesignerChild::openFile( const QString& fileName, const QString& codec )
{
	Q_UNUSED( codec );
	
	if ( QFile::exists( fileName ) )
	{
		// set content
		QFile file( fileName );
		
		if ( !file.open( QIODevice::ReadOnly ) )
		{
			return false;
		}
		
		setWindowTitle( fileName +"[*]" );
		mHostWidget->formWindow()->setContents( &file );
		
		if ( mHostWidget->formWindow()->mainContainer() )
		{
			// set clean
			mHostWidget->formWindow()->setFileName( fileName );
			mHostWidget->formWindow()->setDirty( false );
			
			setWindowModified( false );
			
			emit fileOpened( fileName );
			
			return true;
		}
	}
	
	return false;
}

void QtDesignerChild::closeFile( const QString& s )
{
	emit fileClosed( s );
}

void QtDesignerChild::closeFiles()
{
	closeFile( mHostWidget->formWindow()->fileName() );
}

QStringList QtDesignerChild::files() const
{
	return QStringList( mHostWidget->formWindow()->fileName() );
}

QString QtDesignerChild::fileBuffer( const QString& fileName, bool& ok ) const
{
	Q_UNUSED( fileName );
	ok = false;
	
	if ( mHostWidget->formWindow()->mainContainer() )
	{
		ok = true;
		return mHostWidget->formWindow()->contents();
	}
	
	return QString::null;
}

QString QtDesignerChild::context() const
{
	return PLUGIN_NAME;
}

void QtDesignerChild::initializeContext( QToolBar* tb )
{
	QDesignerFormWindowManagerInterface* fwm = mDesignerManager->core()->formWindowManager();
	
	// add actions to toolbar
	tb->addAction( fwm->actionUndo() );
	tb->addAction( fwm->actionRedo() );
	tb->addAction( fwm->actionCut() );
	tb->addAction( fwm->actionCopy() );
	tb->addAction( fwm->actionPaste() );
	tb->addAction( fwm->actionLower() );
	tb->addAction( fwm->actionRaise() );
	tb->addAction( fwm->actionDelete() );
	tb->addAction( fwm->actionSelectAll() );
	tb->addSeparator();
	
	// tools
	tb->addActions( mDesignerManager->modesActions() );
	tb->addSeparator();
	
	// form
	tb->addAction( fwm->actionHorizontalLayout() );
	tb->addAction( fwm->actionVerticalLayout() );
	tb->addAction( fwm->actionSplitHorizontal() );
	tb->addAction( fwm->actionSplitVertical() );
	tb->addAction( fwm->actionGridLayout() );
	tb->addAction( fwm->actionFormLayout() );
	tb->addAction( fwm->actionSimplifyLayout() );
	tb->addAction( fwm->actionBreakLayout() );
	tb->addAction( fwm->actionAdjustSize() );
	
	// preview
	tb->addSeparator();
	tb->addAction( mDesignerManager->previewFormAction() );
}

QPoint QtDesignerChild::cursorPosition() const
{
	return QPoint( -1, -1 );
}

void QtDesignerChild::showFile( const QString& s )
{
	Q_UNUSED( s );
}

QString QtDesignerChild::currentFile() const
{
	return mHostWidget->formWindow()->fileName();
}

QString QtDesignerChild::currentFileName() const
{
	return QFileInfo( currentFile() ).fileName();
}

bool QtDesignerChild::isModified() const
{
	return mHostWidget->formWindow()->isDirty();
}

bool QtDesignerChild::isUndoAvailable() const
{
	return false;
}

bool QtDesignerChild::isRedoAvailable() const
{
	return false;
}

bool QtDesignerChild::isPasteAvailable() const
{
	return false;
}

bool QtDesignerChild::isCopyAvailable() const
{
	return false;
}

bool QtDesignerChild::isModified( const QString& s ) const
{
	Q_UNUSED( s );
	return isModified();
}

void QtDesignerChild::saveFile( const QString& s )
{
	Q_UNUSED( s );

	// cancel if not modified
	if ( !mHostWidget->formWindow()->isDirty() )
	{
		return;
	}
	
	// write file
	QFile file( mHostWidget->formWindow()->fileName() );
	
	if ( file.open( QIODevice::WriteOnly ) )
	{
		file.resize( 0 );
		file.write( mHostWidget->formWindow()->contents().toUtf8() );
		file.close();
		
		mHostWidget->formWindow()->setDirty( false );
		setWindowModified( false );
		
		emit modifiedChanged( false );
	}
	else
	{
		MonkeyCore::statusBar()->appendMessage( tr( "An error occurs when saving :\n%1" ).arg( s ) );
	}
	
	return;
}

void QtDesignerChild::saveFiles()
{
	saveFile( mHostWidget->formWindow()->fileName() );
}

void QtDesignerChild::printFormHelper( QDesignerFormWindowInterface* form, bool quick )
{
	// get printer
	QPrinter printer;

	// if quick print
	if ( quick )
	{
		// check if default printer is set
		if ( printer.printerName().isEmpty() )
		{
			MonkeyCore::statusBar()->appendMessage( tr( "There is no default printer, please set one before trying quick print" ) );
			return;
		}
		
		// print and return
		QPainter painter( &printer );
		painter.drawPixmap( 0, 0, QPixmap::grabWidget( form ) );
	}
	else
	{
		// printer dialog
		QPrintDialog printDialog( &printer );

		// if ok
		if ( printDialog.exec() )
		{
			// print and return
			QPainter painter( &printer );
			painter.drawPixmap( 0, 0, QPixmap::grabWidget( form ) );
		}
	}
}

void QtDesignerChild::printFile( const QString& s )
{
	Q_UNUSED( s );
	printFormHelper( mHostWidget->formWindow(), false );
}

void QtDesignerChild::quickPrintFile( const QString& s )
{
	Q_UNUSED( s );
	printFormHelper( mHostWidget->formWindow(), true );
}

void QtDesignerChild::undo() {}

void QtDesignerChild::redo() {}

void QtDesignerChild::cut() {}

void QtDesignerChild::copy() {}

void QtDesignerChild::paste() {}

void QtDesignerChild::searchReplace() {}

void QtDesignerChild::goTo() {}

void QtDesignerChild::goTo( const QString& s, const QPoint&, bool )
{
	showFile( s );
}

void QtDesignerChild::backupCurrentFile( const QString& s )
{
	QFile f( s );
	if ( f.open( QIODevice::WriteOnly ) )
	{
		f.resize( 0 );
		f.write( mHostWidget->formWindow()->contents().toUtf8() );
		f.close();
	}
	else
	{
		MonkeyCore::statusBar()->appendMessage( tr( "An error occurs when backuping: %1" ).arg( s ) );
	}
}

bool QtDesignerChild::isSearchReplaceAvailable() const
{
	return false;
}

bool QtDesignerChild::isGoToAvailable() const
{
	return false;
}

bool QtDesignerChild::isPrintAvailable() const
{
	return true;
}
