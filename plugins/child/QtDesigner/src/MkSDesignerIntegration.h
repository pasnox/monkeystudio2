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
#ifndef MKSDESIGNERINTEGRATION_H
#define MKSDESIGNERINTEGRATION_H

#include <QtGlobal>

#if QT_VERSION < 0x050000
#include "qdesigner_integration_p.h"
#else
#include "abstractintegration.h"
#endif

class QDesignerFormEditorInterface;

class MkSDesignerIntegration : public
#if QT_VERSION < 0x050000
qdesigner_internal::
#endif
QDesignerIntegration
{
    Q_OBJECT
    
public:
    MkSDesignerIntegration( QDesignerFormEditorInterface* core, QObject* parent = 0 );
    
    virtual QWidget* containerWindow( QWidget* widget ) const;
};

#endif // MKSDESIGNERINTEGRATION_H
