#ifndef QCTAGSSENSECPP_H
#define QCTAGSSENSECPP_H

#include "qCtagsSenseExport.h"
#include "qCtagsSense.h"

#include <QThread>

class qCtagsSenseSQL;

class QCTAGSSENSE_EXPORT qCtagsSenseKindFinder : public QThread
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
