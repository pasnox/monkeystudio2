#ifndef QCTAGSSENSECPP_H
#define QCTAGSSENSECPP_H

#include <QThread>

#include "qCtagsSense.h"

class qCtagsSenseSQL;

class qCtagsSenseKindFinder : public QThread
{
	Q_OBJECT
	
public:
	qCtagsSenseKindFinder( qCtagsSenseSQL* parent );
	virtual ~qCtagsSenseKindFinder();
	
	void goTo( qCtagsSense::Kind kind, qCtagsSenseEntry* entry );

protected:
	qCtagsSenseSQL* mSQL;
	qCtagsSense::Kind mKind;
	qCtagsSenseEntry* mEntry;
	
	virtual void run();

signals:
	void memberActivated( qCtagsSenseEntry* entry );
};

#endif // QCTAGSSENSECPP_H
