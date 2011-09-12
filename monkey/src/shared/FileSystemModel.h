#ifndef FILESYSTEMMODEL_H
#define FILESYSTEMMODEL_H

#include "coremanager/MonkeyCore.h"

#include <QFileSystemModel>

class Q_MONKEY_EXPORT FileSystemModel : public QFileSystemModel
{
    Q_OBJECT
    
public:
    FileSystemModel( QObject* parent = 0 );
#ifdef Q_OS_MAC
    virtual QVariant data( const QModelIndex& index, int role = Qt::DisplayRole ) const;
#endif

protected:
#ifdef Q_OS_MAC
    static QString MacOSXVolumes;
#endif
};

#endif // FILESYSTEMMODEL_H
