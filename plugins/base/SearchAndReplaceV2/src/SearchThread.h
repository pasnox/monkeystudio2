#ifndef SEARCHTHREAD_H
#define SEARCHTHREAD_H

#include <QThread>
#include <QMutex>
#include <QDir>

#include "SearchWidget.h"
#include "SearchResultsModel.h"

class SearchThread : public QThread
{
	Q_OBJECT
	
public:
	SearchThread( QObject* parent = 0 );
	virtual ~SearchThread();
	
	void search( const SearchWidget::Properties& properties );
	void stop();
	
	SearchWidget::Properties* properties() const;

protected:
	static int mMaxTime;
	SearchWidget::Properties mProperties;
	QMutex mMutex;
	bool mReset;
	bool mExit;
	
	QStringList getFiles( QDir fromDir, const QStringList& filters, bool recursive ) const;
	QStringList getFilesToScan() const;
	QString fileContent( const QString& fileName ) const;
	void search( const QString& fileName, const QString& content ) const;
	virtual void run();

signals:
	void reset();
	void resultsAvailable( const QString& fileName, const SearchResultsModel::ResultList& results );
};

#endif // SEARCHTHREAD_H
