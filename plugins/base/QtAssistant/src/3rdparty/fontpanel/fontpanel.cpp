/****************************************************************************
**
** Copyright (C) 1992-2007 Trolltech ASA. All rights reserved.
**
** This file is part of the tools applications of the Qt Toolkit.
**
** This file may be used under the terms of the GNU General Public
** License version 2.0 as published by the Free Software Foundation
** and appearing in the file LICENSE.GPL included in the packaging of
** this file.  Please review the following information to ensure GNU
** General Public Licensing requirements will be met:
** http://trolltech.com/products/qt/licenses/licensing/opensource/
**
** If you are unsure which license is appropriate for your use, please
** review the following information:
** http://trolltech.com/products/qt/licenses/licensing/licensingoverview
** or contact the sales department at sales@trolltech.com.
**
** In addition, as a special exception, Trolltech gives you certain
** additional rights. These rights are described in the Trolltech GPL
** Exception version 1.0, which can be found at
** http://www.trolltech.com/products/qt/gplexception/ and in the file
** GPL_EXCEPTION.txt in this package.
**
** In addition, as a special exception, Trolltech, as the sole copyright
** holder for Qt Designer, grants users of the Qt/Eclipse Integration
** plug-in the right for the Qt/Eclipse Integration to link to
** functionality provided by Qt Designer and its related libraries.
**
** Trolltech reserves all rights not expressly granted herein.
** 
** Trolltech ASA (c) 2007
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/

#include "fontpanel.h"

#include <QtGui/QLabel>
#include <QtGui/QComboBox>
#include <QtGui/QGridLayout>
#include <QtGui/QSpacerItem>
#include <QtGui/QFontComboBox>
#include <QtCore/QTimer>
#include <QtGui/QLineEdit>


// Add a row consisting of widget and a description label to a grid.
static void addGridRow(const QString &description, QGridLayout *gridLayout, QWidget *w, int &row) {
    QLabel *label = new QLabel(description);
    label->setBuddy(w);
    gridLayout->addWidget(label, row, 0);
    gridLayout->addWidget(w, row, 1);
    ++row;
}

FontPanel::FontPanel(QWidget *parentWidget) :
    QGroupBox(parentWidget),
    m_previewLineEdit(new QLineEdit),
    m_writingSystemComboBox(new QComboBox),
    m_familyComboBox(new QFontComboBox),
    m_styleComboBox(new QComboBox),
    m_pointSizeComboBox(new QComboBox),
    m_previewFontUpdateTimer(0)
{
    setTitle(tr("Font"));

    QGridLayout *gridLayout = new QGridLayout(this);
    int row = 0;

    // writing systems
    m_writingSystemComboBox->setEditable(false);

    QList<QFontDatabase::WritingSystem> writingSystems = m_fontDatabase.writingSystems();
    writingSystems.push_front(QFontDatabase::Any);
    foreach (QFontDatabase::WritingSystem ws, writingSystems)
        m_writingSystemComboBox->addItem(QFontDatabase::writingSystemName(ws), QVariant(ws));
    connect(m_writingSystemComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(slotWritingSystemChanged(int)));
    addGridRow(tr("&Writing system"), gridLayout, m_writingSystemComboBox, row);

    connect(m_familyComboBox, SIGNAL( currentFontChanged(QFont)), this, SLOT(slotFamilyChanged(QFont)));
    addGridRow(tr("&Family"), gridLayout, m_familyComboBox, row);

    m_styleComboBox->setEditable(false);
    connect(m_styleComboBox,  SIGNAL(currentIndexChanged(int)),  this, SLOT(slotStyleChanged(int)));
    addGridRow(tr("&Style"), gridLayout, m_styleComboBox, row);

    m_pointSizeComboBox->setEditable(false);
    connect(m_pointSizeComboBox, SIGNAL(currentIndexChanged(int)),  this, SLOT(slotPointSizeChanged(int)));
    addGridRow(tr("&Point size"), gridLayout, m_pointSizeComboBox, row);

    m_previewLineEdit->setReadOnly(true);
    gridLayout->addWidget (m_previewLineEdit, row, 0, 1, 2);

    setWritingSystem(QFontDatabase::Any);
}

QFont FontPanel::selectedFont() const
{
    QFont rc = m_familyComboBox->currentFont();
    const QString family = rc.family();
    rc.setPointSize(pointSize());
    const QString styleDescription = styleString();
    rc.setItalic(m_fontDatabase.italic(family, styleDescription));

    rc.setBold(m_fontDatabase.bold(family, styleDescription));

    // Weight < 0 asserts...
    const int weight = m_fontDatabase.weight(family, styleDescription);
    if (weight >= 0)
        rc.setWeight(weight);
    return rc;
}

void FontPanel::setSelectedFont(const QFont &f)
{
    m_familyComboBox->setCurrentFont(f);
    if (m_familyComboBox->currentIndex() < 0) {
        // family not in writing system - find the corresponding one?
        QList<QFontDatabase::WritingSystem> familyWritingSystems = m_fontDatabase.writingSystems(f.family());
        if (familyWritingSystems.empty())
            return;

        setWritingSystem(familyWritingSystems.front());
        m_familyComboBox->setCurrentFont(f);
    }

    updateFamily(family());

    const int pointSizeIndex = closestPointSizeIndex(f.pointSize());
    m_pointSizeComboBox->setCurrentIndex( pointSizeIndex);

    const QString styleString = m_fontDatabase.styleString(f);
    const int styleIndex = m_styleComboBox->findText(styleString);
    m_styleComboBox->setCurrentIndex(styleIndex);
    slotUpdatePreviewFont();
}


QFontDatabase::WritingSystem FontPanel::writingSystem() const
{
    const int currentIndex = m_writingSystemComboBox->currentIndex();
    if ( currentIndex == -1)
        return QFontDatabase::Latin;
    return static_cast<QFontDatabase::WritingSystem>(m_writingSystemComboBox->itemData(currentIndex).toInt());
}

QString FontPanel::family() const
{
    const int currentIndex = m_familyComboBox->currentIndex();
    return currentIndex != -1 ?  m_familyComboBox->currentFont().family() : QString();
}

int FontPanel::pointSize() const
{
    const int currentIndex = m_pointSizeComboBox->currentIndex();
    return currentIndex != -1 ? m_pointSizeComboBox->itemData(currentIndex).toInt() : 9;
}

QString FontPanel::styleString() const
{
    const int currentIndex = m_styleComboBox->currentIndex();
    return currentIndex != -1 ? m_styleComboBox->itemText(currentIndex) : QString();
}

void FontPanel::setWritingSystem(QFontDatabase::WritingSystem ws)
{
    m_writingSystemComboBox->setCurrentIndex(m_writingSystemComboBox->findData(QVariant(ws)));
    updateWritingSystem(ws);
}


void FontPanel::slotWritingSystemChanged(int)
{
    updateWritingSystem(writingSystem());
    delayedPreviewFontUpdate();
}

void FontPanel::slotFamilyChanged(const QFont &)
{
    updateFamily(family());
    delayedPreviewFontUpdate();
}

void FontPanel::slotStyleChanged(int)
{
    updatePointSizes(family(), styleString());
    delayedPreviewFontUpdate();
}

void FontPanel::slotPointSizeChanged(int)
{
    delayedPreviewFontUpdate();
}

void FontPanel::updateWritingSystem(QFontDatabase::WritingSystem ws)
{

    m_previewLineEdit->setText(QFontDatabase::writingSystemSample(ws));
    m_familyComboBox->setWritingSystem (ws);
    // Current font not in WS ... set index 0.
    if (m_familyComboBox->currentIndex() < 0) {
        m_familyComboBox->setCurrentIndex(0);
        updateFamily(family());
    }
}

void FontPanel::updateFamily(const QString &family)
{
    // Update styles and trigger update of point sizes.
    // Try to maintain selection or select normal
    const QString oldStyleString = styleString();

    const QStringList styles = m_fontDatabase.styles(family);
    const bool hasStyles = !styles.empty();

    m_styleComboBox->setCurrentIndex(-1);
    m_styleComboBox->clear();
    m_styleComboBox->setEnabled(hasStyles);

    int normalIndex = -1;
    const QString normalStyle = QLatin1String("Normal");

    if (hasStyles) {
        foreach (QString style, styles) {
            // try to maintain selection or select 'normal' preferably
            const int newIndex = m_styleComboBox->count();
            m_styleComboBox->addItem(style);
            if (oldStyleString == style) {
                m_styleComboBox->setCurrentIndex(newIndex);
            } else {
                if (oldStyleString ==  normalStyle)
                    normalIndex = newIndex;
            }
        }
        if (m_styleComboBox->currentIndex() == -1 && normalIndex != -1)
            m_styleComboBox->setCurrentIndex(normalIndex);
    }
    updatePointSizes(family, styleString());
}

int FontPanel::closestPointSizeIndex(int desiredPointSize) const
{
    //  try to maintain selection or select closest.
    int closestIndex = -1;
    int closestAbsError = 0xFFFF;

    const int pointSizeCount = m_pointSizeComboBox->count();
    for (int i = 0; i < pointSizeCount; i++) {
        const int itemPointSize = m_pointSizeComboBox->itemData(i).toInt();
        const int absError = qAbs(desiredPointSize - itemPointSize);
        if (absError < closestAbsError) {
            closestIndex  = i;
            closestAbsError = absError;
            if (closestAbsError == 0)
                break;
        } else {    // past optimum
            if (absError > closestAbsError) {
                break;
            }
        }
    }
    return closestIndex;
}


void FontPanel::updatePointSizes(const QString &family, const QString &styleString)
{
    const int oldPointSize = pointSize();

    QList<int> pointSizes =  m_fontDatabase.pointSizes(family, styleString);
    if (pointSizes.empty())
        pointSizes = QFontDatabase::standardSizes();

    const bool hasSizes = !pointSizes.empty();
    m_pointSizeComboBox->clear();
    m_pointSizeComboBox->setEnabled(hasSizes);
    m_pointSizeComboBox->setCurrentIndex(-1);

    //  try to maintain selection or select closest.
    if (hasSizes) {
        QString n;
        foreach (int pointSize, pointSizes)
            m_pointSizeComboBox->addItem(n.setNum(pointSize), QVariant(pointSize));
        const int closestIndex = closestPointSizeIndex(oldPointSize);
        if (closestIndex != -1)
            m_pointSizeComboBox->setCurrentIndex(closestIndex);
    }
}

void FontPanel::slotUpdatePreviewFont()
{
    m_previewLineEdit->setFont(selectedFont());
}

void FontPanel::delayedPreviewFontUpdate()
{
    if (!m_previewFontUpdateTimer) {
        m_previewFontUpdateTimer = new QTimer(this);
        connect(m_previewFontUpdateTimer, SIGNAL(timeout()), this, SLOT(slotUpdatePreviewFont()));
        m_previewFontUpdateTimer->setInterval(0);
        m_previewFontUpdateTimer->setSingleShot(true);
    }
    if (m_previewFontUpdateTimer->isActive())
        return;

    m_previewFontUpdateTimer->start();
}
