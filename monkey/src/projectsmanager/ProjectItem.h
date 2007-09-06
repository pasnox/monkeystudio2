/****************************************************************************
**
** 		Created using Monkey Studio v1.7.0
** Author    : Nox P@sNox aka Azevedo Filipe <pasnox@gmail.com>
** Project   : ProjectItem
** FileName  : ProjectItem.h
** Date      : 2007-09-04T15:07:31
** License   : GPL
** Comment   : Your comment here
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/
#ifndef PROJECTITEM_H
#define PROJECTITEM_H

#include "MonkeyExport.h"
#include "ProjectsModel.h"

#include <QStandardItem>

class QTextCodec;

class Q_MONKEY_EXPORT ProjectItem : public QStandardItem
{
public:
	ProjectItem( ProjectsModel::NodeType = ProjectsModel::ProjectType, ProjectItem* = 0 );

	// type of the item
	virtual void setType( ProjectsModel::NodeType );
	virtual ProjectsModel::NodeType getType();

	// item operator, only variable
	virtual void setOperator( const QString& );
	virtual QString getOperator() const;

	// item value
	virtual void setValue( const QString& );
	virtual QString getValue() const;

	// item is multiline, only variable
	virtual void setMultiLine( bool );
	virtual bool getMultiLine() const;

	// item modified flag
	virtual void setModified( bool );
	virtual bool getModified() const;

	// item read only, only project
	virtual void setReadOnly( bool );
	virtual bool getReadOnly() const;

	// item content
	virtual void setComment( const QString& );
	virtual QString getComment() const;

	// item filepath
	virtual void setFilePath( const QString& );
	virtual QString getFilePath() const;
	
	// item model
	ProjectsModel* model() const;
	
	// item parent
	ProjectItem* parent() const;
	
	// insert/append items to model
	void appendRow( ProjectItem* );
	void insertRow( int, ProjectItem* );
	
	// the plugin this item come from
	QString pluginName() const { return QString::null; }
	
	// the project item
	ProjectItem* project( const QModelIndex& = QModelIndex() ) const;
	
	// canonical project filepath
	QString canonicalFilePath() const;
	// canonical file path for parameter according to project
	QString canonicalFilePath( const QString& ) const;
	// canonical project path
	QString canonicalPath() const;
	
	// filename of filepath
	QString fileName( const QString& );
	// complete basename of filename
	QString completeBaseName( const QString& );
	
	// get all index matching
	virtual QModelIndexList getIndexList( const QModelIndex&, ProjectsModel::NodeType, const QString&, const QString&, const QString& ) const;
	// get item scope
	virtual QModelIndex getScope( const QModelIndex&, const QString&, bool ) const;
	// get item scope, creating it if not exists
	virtual QString scopeOf( const QModelIndex& ) const;
	// check scope
	virtual QString checkScope( const QString& ) const;
	// check equals scope
	virtual bool isEqualScope( const QString&, const QString& ) const;
	// get all variable content as modelindex list for project index
	virtual QModelIndexList getIndexListValues( const QString&, const QModelIndex& = QModelIndex(), const QString& = "=", const QString& = QString::null ) const;
	// get a variable index
	QModelIndex getIndexVariable( const QString&, const QModelIndex&, const QString&, const QString& ) const;
	// get variable content as stringlist for project index
	virtual QStringList getListValues( const QString&, const QModelIndex& = QModelIndex(), const QString& = "=", const QString& = QString::null ) const;
	// get variable content as string for project index
	virtual QString getStringValues( const QString&, const QModelIndex& = QModelIndex(), const QString& = "=", const QString& = QString::null ) const;
	// set variable content as stringlist for project index
	virtual void setListValues( const QStringList&, const QString&, const QModelIndex& = QModelIndex(), const QString& = "=", const QString& = QString::null );
	// get variable content as string for project index
	virtual void setStringValues( const QString&, const QString&, const QModelIndex& = QModelIndex(), const QString& = "=", const QString& = QString::null );
	// add variable content as stringlist for project index
	virtual void addListValues( const QStringList&, const QString&, const QModelIndex& = QModelIndex(), const QString& = "=", const QString& = QString::null );
	// add variable content as string for project index
	virtual void addStringValues( const QString&, const QString&, const QModelIndex& = QModelIndex(), const QString& = "=", const QString& = QString::null );
	// project codec
	QTextCodec* codec( const QModelIndex& ) const;

};

#endif // PROJECTITEM_H
