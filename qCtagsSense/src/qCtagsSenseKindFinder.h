/****************************************************************************
    Copyright (C) 2005 - 2008  Filipe AZEVEDO & The Monkey Studio Team

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
    
    void goTo( qCtagsSense::Kind kind, const qCtagsSenseEntry* entry );

protected:
    qCtagsSenseSQL* mSQL;
    qCtagsSense::Kind mKind;
    const qCtagsSenseEntry* mEntry;
    
    virtual void run();

signals:
    void entryActivated( const qCtagsSenseEntry& entry );
};

#endif // QCTAGSSENSECPP_H
