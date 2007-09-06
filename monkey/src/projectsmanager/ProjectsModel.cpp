#include "ProjectsModel.h"
#include "ProjectItem.h"

ProjectsModel::ProjectsModel( QObject* o )
	: QStandardItemModel( o )
{
}

ProjectsModel::~ProjectsModel()
{
}

ProjectItem* ProjectsModel::itemFromIndex( const QModelIndex& i )
{ return reinterpret_cast<ProjectItem*>( QStandardItemModel::itemFromIndex( i ) ); }

/*
int ProjectsModel::id() const
{
	return mId;
}
//
ProjectsModel* ProjectsModel::byId( int i )
{
	if ( !mProjectsList.contains( i ) )
		return 0;
	return mProjectsList.value( i );
}
//
QHashProjects ProjectsModel::all()
{
	return mProjectsList;
}
//
bool ProjectsModel::isOpen( const QModelIndex& i ) const
{
	return project( i ).data( ProjectsModel::ProjectOpenRole ).toBool();
}
//
bool ProjectsModel::isModified( const QModelIndex& i ) const
{
	return project( i ).data( ProjectsModel::ProjectModifiedRole ).toBool();
}
//
bool ProjectsModel::isReadOnly( const QModelIndex& i ) const
{
	return project( i ).data( ProjectsModel::ProjectReadOnlyRole ).toBool();
}
//
QsciLexer* ProjectsModel::lexer() const
{
	return mLexer;
}
//
QsciAPIs* ProjectsModel::apis() const
{
	return mAPIs;
}
//
QString ProjectsModel::name( const QModelIndex& i ) const
{
	return QFileInfo( project( i ).data( ProjectsModel::AbsoluteFilePathRole ).toString() ).baseName();
}
//
QString ProjectsModel::path( const QModelIndex& i ) const
{
	return QFileInfo( filePath( i ) ).path();
}
//
QString ProjectsModel::filePath( const QModelIndex& i ) const
{
	return project( i ).data( ProjectsModel::AbsoluteFilePathRole ).toString();
}
//
QString ProjectsModel::filePath( const QString& s, const QModelIndex& i )
{
	return QDir::isRelativePath( s ) ? QFileInfo( QString( "%1/%2" ).arg( path( i ), s ) ).canonicalFilePath() : s;
}
//
QString ProjectsModel::relativeFilePath( const QString& s, const QModelIndex& i )
{
	return QDir::isRelativePath( s ) ? s : QDir( path( i ) ).relativeFilePath( s );
}
//
QStringList ProjectsModel::absoluteFilesPath( const QModelIndex& i, bool b )
{
	qWarning( "ProjectsModel::absoluteFilesPath : may need fix, need to see with hlamer" );
	QStringList l;
	QString s;
	Qt::MatchFlags f = b ? Qt::MatchWildcard | Qt::MatchRecursive : Qt::MatchWildcard;
    qWarning ("ProjectsModel::absoluteFilesPath Segfault if QModelIndex i presented");
	foreach ( QModelIndex j, match( project( i ).child( 0, 0 ), ProjectsModel::AbsoluteFilePathRole, "*", -1, f ) )
	{
		s = j.data( ProjectsModel::AbsoluteFilePathRole ).toString();
		if ( !s.isEmpty() )
			l << s;
	}
	return l;
}
//
QStringList ProjectsModel::absoluteSubProjectsPath( const QModelIndex& i, bool b ) const
{
	QStringList l;
	foreach ( QModelIndex j, subProjects( i, b ) )
		l << j.data( ProjectsModel::AbsoluteFilePathRole ).toString();
	return l;
}
//
QModelIndex ProjectsModel::project( const QModelIndex& i ) const
{
	QModelIndex j = i.isValid() ? i : rootProject();
	while ( j.data( ProjectsModel::TypeRole ).toInt() != ProjectsModel::ProjectType && j.isValid() )
		j = j.parent();
	if ( !j.isValid() )
		j = rootProject();
	return j;
}
//
#include <QDebug>
QModelIndexList ProjectsModel::subProjects( const QModelIndex& i, bool b ) const
{
	Qt::MatchFlags f = b ? Qt::MatchFixedString | Qt::MatchRecursive : Qt::MatchFixedString;
	return match( project( i ).child( 0, 0 ), ProjectsModel::TypeRole, ProjectsModel::ProjectType, -1, f );
}
//
QModelIndex ProjectsModel::parentProject( const QModelIndex& i ) const
{
	return project( project( i ).parent() );
}
//
QTextCodec* ProjectsModel::defaultCodec() const
{
	return QTextCodec::codecForName( "System" );
}
//
void ProjectsModel::setFilePath( const QString& s, const QModelIndex& i )
{
	QModelIndex j = project( i );
	QString cfp = QFileInfo( s ).canonicalFilePath();
	if ( !j.isValid() || isOpen( j ) || !QFile::exists( cfp ) )
		return;
	setData( j, cfp, ProjectsModel::AbsoluteFilePathRole );
	qWarning( "ProjectsModel::setFilePath : Does it need to fill value, and item does the rest ?!" );
}
//
void ProjectsModel::setModified( const QModelIndex& i, bool b )
{
	QModelIndex j = project( i );
	if ( !j.isValid() )
		return;
	setData( j, b, ProjectsModel::ProjectModifiedRole );
	emit isModifiedChanged( b, j );
}
//
void ProjectsModel::setReadOnly( const QModelIndex& i, bool b )
{
	QModelIndex j = project( i );
	if ( !j.isValid() )
		return;
	setData( j, b, ProjectsModel::ProjectReadOnlyRole );
	emit isReadOnlyChanged( b, j );
}
//
void ProjectsModel::setLexer( QsciLexer* l )
{
	delete mLexer;
	mLexer = l;
}
//
void ProjectsModel::setAPIs( QsciAPIs* a )
{
	delete mAPIs;
	mAPIs = a;
}
// get all files
QStringList ProjectsModel::getFiles( QDir d, const QStringList& f, bool b )
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
*/