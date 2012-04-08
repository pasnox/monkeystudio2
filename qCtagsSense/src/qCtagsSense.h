#ifndef QCTAGSSENSE_H
#define QCTAGSSENSE_H

#include "qCtagsSenseExport.h"
#include "qCtagsSenseProperties.h"

#include <QObject>

namespace qCtagsSense
{

class QCTAGSSENSE_EXPORT Instance : public QObject
{
    Q_OBJECT
    
public:
    Instance( QObject* parent = 0 );
    virtual ~Instance();

protected:
    qCtagsSense::Properties mProperties;
};

}; // namespace qCtagsSense

#endif // QCTAGSSENSE_H
