#ifndef QTIMETRACKER_H
#define QTIMETRACKER_H

#include <QTime>

class QTimeTracker : public QTime
{
public:
	QTimeTracker( const QString& name );

public:
	void query() const;

protected:
	mutable int mAutoIncrement;
	QString mName;
};

#endif // QTIMETRACKER_H
