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
#ifndef FORMSCRIPTRUNNER_H
#define FORMSCRIPTRUNNER_H

//
//  W A R N I N G
//  -------------
//
// This file is not part of the Qt API.  It exists purely as an
// implementation detail.  This header file may change from version to
// version without notice, or even be removed.
//
// We mean it.
//

#include <QtDesigner/uilib_global.h>
#include <QtCore/QList>
#include <QtCore/QFlags>
#include <QtCore/QString>

QT_BEGIN_NAMESPACE

class QWidget;

#ifdef QFORMINTERNAL_NAMESPACE
namespace QFormInternal
{
#endif

class DomWidget;

class QDESIGNER_UILIB_EXPORT QFormScriptRunner
{
public:
    QFormScriptRunner();
    ~QFormScriptRunner();

    typedef QList<QWidget*> WidgetList;

    bool run(const DomWidget *domWidget,
             const QString &customWidgetScript,
             QWidget *widget, const WidgetList &children,
             QString *errorMessage);

    struct Error {
        QString objectName;
        QString script;
        QString errorMessage;
    };
    typedef QList<Error> Errors;
    Errors errors() const;
    void clearErrors();

    enum Option {
         NoOptions = 0x0,
         DisableWarnings = 0x1,
         DisableScripts = 02
     };
     Q_DECLARE_FLAGS(Options, Option)

    Options options() const;
    void setOptions(Options options);

private:
    class QFormScriptRunnerPrivate;
    QFormScriptRunnerPrivate *m_impl;

    QFormScriptRunner(const QFormScriptRunner &);
    void operator = (const QFormScriptRunner &);
};

Q_DECLARE_OPERATORS_FOR_FLAGS(QFormScriptRunner::Options)

#ifdef QFORMINTERNAL_NAMESPACE
}
#endif

QT_END_NAMESPACE

#endif // FORMSCRIPTRUNNER_H
