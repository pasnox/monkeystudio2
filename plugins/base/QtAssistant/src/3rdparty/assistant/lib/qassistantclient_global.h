/****************************************************************************
**
** Copyright (C) 1992-2006 Trolltech ASA. All rights reserved.
**
** This file is part of the Qt Assistant of the Qt Toolkit.
**
** This file may be used under the terms of the GNU General Public
** License version 2.0 as published by the Free Software Foundation
** and appearing in the file LICENSE.GPL included in the packaging of
** this file.  Please review the following information to ensure GNU
** General Public Licensing requirements will be met:
** http://www.trolltech.com/products/qt/opensource.html
**
** If you are unsure which license is appropriate for your use, please
** review the following information:
** http://www.trolltech.com/products/qt/licensing.html or contact the
** sales department at sales@trolltech.com.
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/

#ifndef QASSISTANTCLIENT_GLOBAL_H
#define QASSISTANTCLIENT_GLOBAL_H

#include <QtCore/qglobal.h>

QT_BEGIN_HEADER

#if defined(QT_ASSISTANT_CLIENT_STATIC) || !defined(QT_DLL)
#  define QT_ASSISTANT_CLIENT_EXPORT
#elif defined(QT_ASSISTANT_CLIENT_LIBRARY)
#  define QT_ASSISTANT_CLIENT_EXPORT Q_DECL_EXPORT
#else
#  define QT_ASSISTANT_CLIENT_EXPORT Q_DECL_IMPORT
#endif

QT_END_HEADER

#endif
