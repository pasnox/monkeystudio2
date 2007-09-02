#include "AbstractProjectModel.h"
//
#include <QHash>
#include <QFileInfo>
#include <QTextCodec>
//
#include <qscilexer.h>
#include <qsciapis.h>
//
int AbstractProjectModel::mUniqueId = 0;
QHashProjects AbstractProjectModel::mProjectsList = QHashProjects();
//
AbstractProjectModel::AbstractProjectModel( const QString& s, QObject* p )
	: QAbstractItemModel( p ), mId( mUniqueId++ ), mRootFilePath( s ), mLexer( 0 ), mAPIs( 0 )
{
	mProjectsList[ mId ] = this;
}
//
AbstractProjectModel::~AbstractProjectModel()
{
	// remove itself from projects list
	if ( mProjectsList.contains( mId ) )
		mProjectsList.remove( mId );
	// delete lexer and proxy
	delete mAPIs;
	delete mLexer;
}
//
int AbstractProjectModel::id() const
{
	return mId;
}
//
AbstractProjectModel* AbstractProjectModel::byId( int i )
{
	if ( !mProjectsList.contains( i ) )
		return 0;
	return mProjectsList.value( i );
}
//
QHashProjects AbstractProjectModel::all()
{
	return mProjectsList;
}
//
bool AbstractProjectModel::isOpen( const QModelIndex& i ) const
{
	return project( i ).data( AbstractProjectModel::ProjectOpenRole ).toBool();
}
//
bool AbstractProjectModel::isModified( const QModelIndex& i ) const
{
	return project( i ).data( AbstractProjectModel::ProjectModifiedRole ).toBool();
}
//
bool AbstractProjectModel::isReadOnly( const QModelIndex& i ) const
{
	return project( i ).data( AbstractProjectModel::ProjectReadOnlyRole ).toBool();
}
//
QsciLexer* AbstractProjectModel::lexer() const
{
	return mLexer;
}
//
QsciAPIs* AbstractProjectModel::apis() const
{
	return mAPIs;
}
//
QString AbstractProjectModel::name( const QModelIndex& i ) const
{
	return QFileInfo( project( i ).data( AbstractProjectModel::AbsoluteFilePathRole ).toString() ).baseName();
}
//
QString AbstractProjectModel::path( const QModelIndex& i ) const
{
	return QFileInfo( filePath( i ) ).path();
}
//
QString AbstractProjectModel::filePath( const QModelIndex& i ) const
{
	return project( i ).data( AbstractProjectModel::AbsoluteFilePathRole ).toString();
}
//
QString AbstractProjectModel::filePath( const QString& s, const QModelIndex& i )
{
	return QDir::isRelativePath( s ) ? QFileInfo( QString( "%1/%2" ).arg( path( i ), s ) ).canonicalFilePath() : s;
}
//
QString AbstractProjectModel::relativeFilePath( const QString& s, const QModelIndex& i )
{
	return QDir::isRelativePath( s ) ? s : QDir( path( i ) ).relativeFilePath( s );
}
//
QStringList AbstractProjectModel::absoluteFilesPath( const QModelIndex& i, bool b )
{
	qWarning( "AbstractProjectModel::absoluteFilesPath : may need fix, need to see with hlamer" );
	QStringList l;
	QString s;
	Qt::MatchFlags f = b ? Qt::MatchWildcard | Qt::MatchRecursive : Qt::MatchWildcard;
    qWarning ("AbstractProjectModel::absoluteFilesPath Segfault if QModelIndex i presented");
	foreach ( QModelIndex j, match( project( i ).child( 0, 0 ), AbstractProjectModel::AbsoluteFilePathRole, "*", -1, f ) )
	{
		s = j.data( AbstractProjectModel::AbsoluteFilePathRole ).toString();
		if ( !s.isEmpty() )
			l << s;
	}
	return l;
}
//
QStringList AbstractProjectModel::absoluteSubProjectsPath( const QModelIndex& i, bool b ) const
{
	QStringList l;
	foreach ( QModelIndex j, subProjects( i, b ) )
		l << j.data( AbstractProjectModel::AbsoluteFilePathRole ).toString();
	return l;
}
//
QModelIndex AbstractProjectModel::project( const QModelIndex& i ) const
{
	QModelIndex j = i.isValid() ? i : rootProject();
	while ( j.data( AbstractProjectModel::TypeRole ).toInt() != AbstractProjectModel::ProjectType && j.isValid() )
		j = j.parent();
	if ( !j.isValid() )
		j = rootProject();
	return j;
}
//
#include <QDebug>
QModelIndexList AbstractProjectModel::subProjects( const QModelIndex& i, bool b ) const
{
	Qt::MatchFlags f = b ? Qt::MatchFixedString | Qt::MatchRecursive : Qt::MatchFixedString;
	return match( project( i ).child( 0, 0 ), AbstractProjectModel::TypeRole, AbstractProjectModel::ProjectType, -1, f );
}
//
QModelIndex AbstractProjectModel::parentProject( const QModelIndex& i ) const
{
	return project( project( i ).parent() );
}
//
QTextCodec* AbstractProjectModel::defaultCodec() const
{
	return QTextCodec::codecForName( "System" );
}
//
void AbstractProjectModel::setFilePath( const QString& s, const QModelIndex& i )
{
	QModelIndex j = project( i );
	QString cfp = QFileInfo( s ).canonicalFilePath();
	if ( !j.isValid() || isOpen( j ) || !QFile::exists( cfp ) )
		return;
	setData( j, cfp, AbstractProjectModel::AbsoluteFilePathRole );
	qWarning( "AbstractProjectModel::setFilePath : Does it need to fill value, and item does the rest ?!" );
}
//
void AbstractProjectModel::setModified( const QModelIndex& i, bool b )
{
	QModelIndex j = project( i );
	if ( !j.isValid() )
		return;
	setData( j, b, AbstractProjectModel::ProjectModifiedRole );
	emit isModifiedChanged( b, j );
}
//
void AbstractProjectModel::setReadOnly( const QModelIndex& i, bool b )
{
	QModelIndex j = project( i );
	if ( !j.isValid() )
		return;
	setData( j, b, AbstractProjectModel::ProjectReadOnlyRole );
	emit isReadOnlyChanged( b, j );
}
//
void AbstractProjectModel::setLexer( QsciLexer* l )
{
	delete mLexer;
	mLexer = l;
}
//
void AbstractProjectModel::setAPIs( QsciAPIs* a )
{
	delete mAPIs;
	mAPIs = a;
}
// get all files
QStringList AbstractProjectModel::getFiles( QDir d, const QStringList& f, bool b )
{
	QStringList l;
	if ( !d.exists() )
		return l;
	// looking recursive fodlers to load if needed
	if ( b )
	{
		foreach ( QString s, d.entryList( QDir::Dirs | QDir::NoDotAndDotDot ) )
		{
			d.cd( s );
			l << getFiles( d, f, b );
			d.cdUp();
		}
	}
	// looking files to load
	foreach ( QString s, d.entryList( f, QDir::Files ) )
		l << QDir( path() ).relativeFilePath( d.absoluteFilePath( s ) );
	//
	return l;
}
