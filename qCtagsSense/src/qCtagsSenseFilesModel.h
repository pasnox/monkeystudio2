#ifndef QCTAGSSENSEFILESMODEL_H
#define QCTAGSSENSEFILESMODEL_H

#include <QAbstractItemModel>
#include <QStringList>

class qCtagsSenseSQL;
class qCtagsSenseFilesThread;

class qCtagsSenseFilesModel : public QAbstractItemModel
{
	Q_OBJECT
	
public:
	qCtagsSenseFilesModel( qCtagsSenseSQL* parent );
	virtual ~qCtagsSenseFilesModel();
	
	virtual int columnCount( const QModelIndex& parent = QModelIndex() ) const;
	virtual QVariant data( const QModelIndex& index, int role = Qt::DisplayRole ) const;
	virtual QModelIndex index( int row, int column, const QModelIndex& parent = QModelIndex() ) const;
	virtual QModelIndex parent( const QModelIndex& index ) const;
	virtual int rowCount( const QModelIndex& parent = QModelIndex() ) const;
	virtual bool hasChildren( const QModelIndex& parent = QModelIndex() ) const;
	
	QString fileName( int id ) const;
	int indexOf( const QString& fileName ) const;

public slots:
	void refresh( const QString& language );

protected:
	qCtagsSenseFilesThread* mThread;
	qCtagsSenseSQL* mSQL;
	QStringList* mFiles;

protected slots:
	void queryFinished( const QStringList& files );

signals:
	void ready();
};

#endif // QCTAGSSENSEFILESMODEL_H
