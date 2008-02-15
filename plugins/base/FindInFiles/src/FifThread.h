#ifndef FIFTHREAD_H
#define FIFTHREAD_H

#include <QThread>
#include <QStringList>

class FifThread : public QThread
{
	Q_OBJECT
public:
	FifThread(const QStringList &, const QString &, bool, bool, bool, QObject* parent);
	~FifThread();
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
	void sig_updateResult(uint, const QString&, const QString&);
	void sig_changeProgress(int);
};

#endif //FIFTHREAD_H
