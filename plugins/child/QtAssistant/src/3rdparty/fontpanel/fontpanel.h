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
// of the Qt tools.  This header
// file may change from version to version without notice, or even be removed.
//
// We mean it.
//

#ifndef FONTPANEL_H
#define FONTPANEL_H

#include <QtGui/QGroupBox>
#include <QtGui/QFont>
#include <QtGui/QFontDatabase>

QT_BEGIN_NAMESPACE

class QComboBox;
class QFontComboBox;
class QTimer;
class QLineEdit;

class FontPanel: public QGroupBox
{
    Q_OBJECT
public:
    FontPanel(QWidget *parentWidget = 0);

    QFont selectedFont() const;
    void setSelectedFont(const QFont &);

    QFontDatabase::WritingSystem writingSystem() const;
    void setWritingSystem(QFontDatabase::WritingSystem ws);

private slots:
    void slotWritingSystemChanged(int);
    void slotFamilyChanged(const QFont &);
    void slotStyleChanged(int);
    void slotPointSizeChanged(int);
    void slotUpdatePreviewFont();

private:
    QString family() const;
    QString styleString() const;
    int pointSize() const;
    int closestPointSizeIndex(int ps) const;

    void updateWritingSystem(QFontDatabase::WritingSystem ws);
    void updateFamily(const QString &family);
    void updatePointSizes(const QString &family, const QString &style);
    void delayedPreviewFontUpdate();

    QFontDatabase m_fontDatabase;
    QLineEdit *m_previewLineEdit;
    QComboBox *m_writingSystemComboBox;
    QFontComboBox* m_familyComboBox;
    QComboBox *m_styleComboBox;
    QComboBox *m_pointSizeComboBox;
    QTimer *m_previewFontUpdateTimer;
};

QT_END_NAMESPACE

#endif // FONTPANEL_H
