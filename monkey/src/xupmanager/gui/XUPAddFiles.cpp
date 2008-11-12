#include "XUPAddFiles.h"
#include "XUPProjectModelProxy.h"
#include "XUPProjectModel.h"
#include "XUPItem.h"
#include "XUPProjectItem.h"
#include "pMonkeyStudio.h"

XUPAddFiles::XUPAddFiles( QWidget* parent )
	: QWidget( parent )
{
	setupUi( this );
	
	mModel = 0;
	mProxy = new XUPProjectModelProxy( this );
	tcbScopes->setModel( mProxy );
}

XUPAddFiles::~XUPAddFiles()
{
}

void XUPAddFiles::on_tcbScopes_currentChanged( const QModelIndex& index )
{
	Q_UNUSED( index );
	XUPItem* scope = currentScope();
	
	QString curText = cbImport->currentText();
	cbImport->clear();
	
	if ( scope )
	{
		QDir dir( scope->project()->path() );
		
		foreach ( const QFileInfo& fi, pMonkeyStudio::getFolders( dir, QStringList() ) )
		{
			cbImport->addItem( dir.relativeFilePath( fi.filePath() ) );
		}
		
		int id = cbImport->findText( curText );
		cbImport->setCurrentIndex( id );
	}
	
	emit currentScopeChanged( scope );
}

void XUPAddFiles::setModel( XUPProjectModel* model )
{
	if ( mModel != model )
	{
		if ( mModel )
		{
			// disconnect
		}
		
		mModel = model;
		mProxy->setSourceModel( mModel );
		
		if ( mModel )
		{
			// connect
		}
	}
}

XUPProjectModel* XUPAddFiles::model() const
{
	return mModel;
}

void XUPAddFiles::setAddToProjectChoice( bool choice )
{
	if ( choice )
	{
		gbScopes->setCheckable( true );
	}
	else
	{
		gbScopes->setCheckable( false );
	}
}

bool XUPAddFiles::addToProjectChoice() const
{
	return gbScopes->isCheckable();
}

bool XUPAddFiles::addToProject() const
{
	if ( gbScopes->isCheckable() )
	{
		return gbScopes->isChecked();
	}
	
	return true;
}

void XUPAddFiles::setCurrentScope( XUPItem* item )
{
	QModelIndex index = item->index();
	index = mProxy->mapFromSource( index );
	tcbScopes->setCurrentIndex( index );
}

XUPItem* XUPAddFiles::currentScope() const
{
	QModelIndex index = tcbScopes->currentIndex();
	index = mProxy->mapToSource( index );
	return mModel->itemFromIndex( index );
}

void XUPAddFiles::setOperators( const QStringList& operators )
{
	cbOperators->clear();
	cbOperators->addItems( operators );
}

void XUPAddFiles::setCurrentOperator( const QString& op )
{
	cbOperators->setCurrentIndex( cbOperators->findText( op ) );
}

QString XUPAddFiles::currentOperator() const
{
	return cbOperators->currentText();
}

void XUPAddFiles::setImportExternalFiles( bool import )
{
	gbImport->setChecked( import );
}

bool XUPAddFiles::importExternalFiles() const
{
	return gbImport->isChecked();
}

void XUPAddFiles::setImportExternalFilesPath( const QString& path )
{
	int id = cbImport->findText( path );
	
	if ( id == -1 && QFileInfo( path ).isRelative() )
	{
		cbImport->addItem( path );
		id = cbImport->findText( path );
	}
	
	if ( id != -1 )
	{
		cbImport->setCurrentIndex( id );
	}
}

QString XUPAddFiles::importExternalFilesPath() const
{
	return cbImport->currentText();
}
