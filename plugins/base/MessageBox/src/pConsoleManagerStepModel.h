/****************************************************************************
    Copyright (C) 2005 - 2011  Filipe AZEVEDO & The Monkey Studio Team
    http://monkeystudio.org licensing under the GNU GPL.

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
****************************************************************************/
#ifndef PCONSOLEMANAGERSTEPMODEL_H
#define PCONSOLEMANAGERSTEPMODEL_H

#include <QAbstractItemModel>

#include <MonkeyExport.h>

#include <consolemanager/pConsoleManagerStep.h>

class pConsoleManagerStepModel : public QAbstractItemModel
{
    Q_OBJECT
    
public:
    pConsoleManagerStepModel( QObject* parent = 0 );
    virtual ~pConsoleManagerStepModel();
    
    virtual int columnCount( const QModelIndex& parent = QModelIndex() ) const;
    virtual QVariant data( const QModelIndex& index, int role = Qt::DisplayRole ) const;
    virtual QModelIndex index( int row, int column, const QModelIndex& parent = QModelIndex() ) const;
    virtual QModelIndex parent( const QModelIndex& index ) const;
    virtual int rowCount( const QModelIndex& parent = QModelIndex() ) const;
    
    virtual bool hasChildren( const QModelIndex& parent = QModelIndex() ) const;
    
    QModelIndex index( const pConsoleManagerStep& step ) const;
    pConsoleManagerStep step( const QModelIndex& index ) const;
    
    QModelIndex nextErrorOrWarning( const QModelIndex& fromIndex ) const;
    QModelIndex nextWarning( const QModelIndex& fromIndex ) const;
    QModelIndex nextError( const QModelIndex& fromIndex ) const;

public slots:
    void clear();
    void appendStep( const pConsoleManagerStep& step );
    void appendSteps( const pConsoleManagerStepList& steps );

protected:
    mutable pConsoleManagerStepList mSteps;
    uint mMessages;
    uint mWarnings;
    uint mErrors;
};

#endif // PCONSOLEMANAGERSTEPMODEL_H
