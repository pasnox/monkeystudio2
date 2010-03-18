#ifndef QCTAGSSENSE_H
#define QCTAGSSENSE_H

#include "qCtagsSenseExport.h"

#include <QObject>

class QCTAGSSENSE_EXPORT qCtagsSense : public QObject
{
	Q_OBJECT
	
public:
	qCtagsSense( QObject* parent = 0 );
	virtual ~qCtagsSense();
};

#endif // QCTAGSSENSE_H
