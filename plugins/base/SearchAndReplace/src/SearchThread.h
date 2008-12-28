#ifndef SEARCHTHREAD_H
#define SEARCHTHREAD_H

#include "SearchAndReplace.h"

#include <QThread>
#include <QStringList>
#include <QMutex>
#include <QTimer>

class QFile;

class SearchThread : public QThread
{
    Q_OBJECT
public:
	enum Mode
	{
		SEARCH,
		REPLACE
	};
	
    SearchThread(Mode mode, const QString &dir, QString &mask, const QString &text, const QString& replace, bool caseSensitive, bool regExp, QObject* parent);
    ~SearchThread();
    void run();
    void setTermEnabled(bool t) {mTerm = t;};
    bool isTermEnabled() {return mTerm;};
	
	void lockResultsAccessMutex ();
	void unlockResultsAccessMutex ();
	// NOTE it's nessesary to lock mutex before use this methods
	QList<SearchAndReplace::Occurence> newFoundOccurences();
	void clearNewFoundOccurences();
	int processedFilesCount();
	int foundOccurencesCount();
	
private:
    QString mDir;
    QString mMask;
    QString mSearch;
	QString mReplace;
    bool mIsWhole;
    bool mIsMatch;
    bool mIsReg;
	bool mCaseSensetive;
	
	Mode mMode;
	
    bool mTerm;
	
	// results
	QMutex mResultsAccessMutex;
	int mProcessedFilesCount;
	int mOccurencesFound;
	QList<SearchAndReplace::Occurence> mNewFoundOccurences;
	
	QTimer mReadPleaseResultsTimer;
	
	// Heuristics: if first 1k of file contains '\0' - file is binary
	// NOTE: procedure moving current pos in the file
	bool isBinary (QFile& file);
	
	void search (QFile& file);
	void replace (QFile& file);
	
signals:
    void readPleaseResults ();
};

#endif //SEARCHTHREAD_H
