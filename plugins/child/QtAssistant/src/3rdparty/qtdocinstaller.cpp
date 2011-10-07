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
#include <QtCore/QDir>
#include <QtCore/QLibraryInfo>
#include <QtCore/QDateTime>
#include <QtHelp/QHelpEngineCore>
#include "qtdocinstaller.h"

QT_BEGIN_NAMESPACE

QtDocInstaller::QtDocInstaller(const QString &collectionFile)
{
    m_abort = false;
    m_collectionFile = collectionFile;
}

QtDocInstaller::~QtDocInstaller()
{
    if (!isRunning())
        return;
    m_mutex.lock();
    m_abort = true;
    m_mutex.unlock();
    wait();
}

void QtDocInstaller::installDocs()
{
    start(LowPriority);
}

void QtDocInstaller::run()
{
    QHelpEngineCore *helpEngine = new QHelpEngineCore(m_collectionFile);
    helpEngine->setupData();
    bool changes = false;

    QStringList docs;
    docs << QLatin1String("assistant")
        << QLatin1String("designer")
        << QLatin1String("linguist")
        << QLatin1String("qmake")
        << QLatin1String("qt");

    foreach (const QString &doc, docs) {
        changes |= installDoc(doc, helpEngine);
        m_mutex.lock();
        if (m_abort) {
            delete helpEngine;
            m_mutex.unlock();
            return;
        }
        m_mutex.unlock();
    }
    delete helpEngine;
    emit docsInstalled(changes);
}

bool QtDocInstaller::installDoc(const QString &name, QHelpEngineCore *helpEngine)
{
    QString versionKey = QString(QLatin1String("qtVersion%1$$$%2")).
        arg(QLatin1String(QT_VERSION_STR)).arg(name);

    QString info = helpEngine->customValue(versionKey, QString()).toString();
    QStringList lst = info.split(QLatin1String("|"));

    QDateTime dt;
    if (lst.count() && !lst.first().isEmpty())
        dt = QDateTime::fromString(lst.first(), Qt::ISODate);

    QString qchFile;
    if (lst.count() == 2)
        qchFile = lst.last();

    QDir dir(QLibraryInfo::location(QLibraryInfo::DocumentationPath)
        + QDir::separator() + QLatin1String("qch"));

    const QStringList files = dir.entryList(QStringList() << QLatin1String("*.qch"));
    if (files.isEmpty()) {
        helpEngine->setCustomValue(versionKey, QDateTime().toString(Qt::ISODate)
            + QLatin1String("|"));
        return false;
    }
    foreach (const QString &f, files) {
        if (f.startsWith(name)) {
            QFileInfo fi(dir.absolutePath() + QDir::separator() + f);
            if (dt.isValid() && fi.lastModified().toString(Qt::ISODate) == dt.toString(Qt::ISODate)
                && qchFile == fi.absoluteFilePath())
                return false;

            QString namespaceName = QHelpEngineCore::namespaceName(fi.absoluteFilePath());
            if (namespaceName.isEmpty())
                continue;

            if (helpEngine->registeredDocumentations().contains(namespaceName))
                helpEngine->unregisterDocumentation(namespaceName);

            if (!helpEngine->registerDocumentation(fi.absoluteFilePath())) {
                emit errorMessage(
                    tr("The file %1 could not be registered successfully!\n\nReason: %2")
                    .arg(fi.absoluteFilePath()).arg(helpEngine->error()));
            }

            helpEngine->setCustomValue(versionKey, fi.lastModified().toString(Qt::ISODate)
                + QLatin1String("|") + fi.absoluteFilePath());
            return true;
        }
    }
    return false;
}

QT_END_NAMESPACE
