#ifndef SEARCHTHREAD_H
#define SEARCHTHREAD_H

#include "pConsoleManager.h"

#include <QThread>
#include <QStringList>

class SearchThread : public QThread
{
	Q_OBJECT
public:
	SearchThread(const QStringList &, const QString &, bool, bool, bool, QObject* parent);
	~SearchThread();
	void run();
	void setTermEnabled(bool t) {mTerm = t;};
	bool isTermEnabled() {return mTerm;};
private:
	QStringList files;
	QString text; 
	bool isWhole;
	bool isMatch;
	bool isReg;
	bool mTerm;
signals:
	void appendSearchResult( const pConsoleManager::Step& );
	void changeProgress(int);
};

#endif //SEARCHTHREAD_H
