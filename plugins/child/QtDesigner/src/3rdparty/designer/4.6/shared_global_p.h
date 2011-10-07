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
//
//  W A R N I N G
//  -------------
//
// This file is not part of the Qt API.  It exists for the convenience
// of Qt Designer.  This header
// file may change from version to version without notice, or even be removed.
//
// We mean it.
//

#ifndef SHARED_GLOBAL_H
#define SHARED_GLOBAL_H

#include <QtCore/qglobal.h>

#ifdef QT_DESIGNER_STATIC
#define QDESIGNER_SHARED_EXTERN
#define QDESIGNER_SHARED_IMPORT
#else
#define QDESIGNER_SHARED_EXTERN Q_DECL_EXPORT
#define QDESIGNER_SHARED_IMPORT Q_DECL_IMPORT
#endif

#ifndef QT_NO_SHARED_EXPORT
#  ifdef QDESIGNER_SHARED_LIBRARY
#    define QDESIGNER_SHARED_EXPORT QDESIGNER_SHARED_EXTERN
#  else
#    define QDESIGNER_SHARED_EXPORT QDESIGNER_SHARED_IMPORT
#  endif
#else
#  define QDESIGNER_SHARED_EXPORT
#endif

#endif // SHARED_GLOBAL_H
