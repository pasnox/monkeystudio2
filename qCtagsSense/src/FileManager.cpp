#include "FileManager.h"
#include "qCtagsSense.h"

#include <QPlainTextEdit>
#include <QFile>
#include <QTextBlock>
#include <QTimer>
#include <QDateTime>

#include <QDebug>

const int INDEX_TIME_ELAPSE = 1000;

FileManager::FileManager( QObject* parent )
	: QObject( parent )
{
	mFileModifiedTimer = new QTimer( this );
	connect( mFileModifiedTimer, SIGNAL( timeout() ), this, SLOT( internal_bufferModified_timeout() ) );
}

FileManager::~FileManager()
{
	qDeleteAll( mEditors );
}

QPlainTextEdit* FileManager::openFile( const QString& fileName )
{
	QPlainTextEdit* editor = mEditors[ fileName ];
	
	if ( !editor && QFile::exists( fileName ) )
	{
		mEditors[ fileName ] = ( editor = new QPlainTextEdit );
		editor->setAttribute( Qt::WA_DeleteOnClose );
		editor->resize( 640, 480 );
		
		QFile file( fileName );
		file.open( QFile::ReadOnly );
		
		editor->setPlainText( QString::fromUtf8( file.readAll() ) );
		editor->document()->setModified( false );
		editor->setWindowFilePath( fileName );
		
		connect( editor, SIGNAL( textChanged() ), this, SLOT( bufferModified() ) );
		
		editor->show();
	}
	
	return editor;
}

void FileManager::goTo( const QString& fileName, const QPoint& pos )
{
	QPlainTextEdit* editor = openFile( fileName );
	
	if ( editor )
	{
		QTextBlock block = editor->document()->findBlockByNumber( pos.y() );
		QTextCursor cursor( block );
		
		if ( cursor.atBlockStart() || cursor.movePosition( QTextCursor::StartOfBlock ) )
		{
			if ( cursor.movePosition( QTextCursor::EndOfBlock, QTextCursor::KeepAnchor ) )
			{
				editor->setTextCursor( cursor );
			}
		}
		
		editor->raise();
		editor->activateWindow();
	}
}

void FileManager::memberActivated( qCtagsSenseEntry* entry )
{	
	goTo( entry->fileName, QPoint( 0, entry->lineNumber -1 ) );
}

void FileManager::bufferModified()
{
	mFileModifiedTimer->start( INDEX_TIME_ELAPSE );
}

void FileManager::internal_bufferModified_timeout()
{
	mFileModifiedTimer->stop();
	
	QMap<QString, QString> mBuffers;
	
	foreach ( QPlainTextEdit* editor, mEditors )
	{
		if ( editor )
		{
			QDateTime lastCheck = editor->property( "lastIndexDateTime" ).toDateTime();
			QDateTime now = QDateTime::currentDateTime();
			
			if ( !lastCheck.isValid() || lastCheck.secsTo( now ) > INDEX_TIME_ELAPSE /1000 )
			{
				mBuffers[ editor->windowFilePath() ] = editor->toPlainText();
				editor->setProperty( "lastIndexDateTime", now );
			}
		}
	}
	
	if ( !mBuffers.isEmpty() )
	{
		emit buffersModified( mBuffers );
	}
}
