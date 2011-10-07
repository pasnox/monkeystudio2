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
#ifndef QTASSISTANTINLINESEARCH_H
#define QTASSISTANTINLINESEARCH_H

#include <QWidget>

class QLineEdit;
class QCheckBox;
class QLabel;
class QToolButton;

class QtAssistantInlineSearch : public QWidget
{
    Q_OBJECT
    friend class QtAssistantChild;

public:
    QtAssistantInlineSearch( QWidget* parent = 0 );

protected:
    void keyPressEvent( QKeyEvent* event );

private slots:
    void updateButtons();

private:
    QLineEdit* editFind;
    QCheckBox* checkCase;
    QLabel* labelWrapped;
    QToolButton* toolNext;
    QToolButton* toolClose;
    QToolButton* toolPrevious;
    QCheckBox* checkWholeWords;

signals:
    void findNext();
    void findPrevious();
};

#endif // QTASSISTANTINLINESEARCH_H
