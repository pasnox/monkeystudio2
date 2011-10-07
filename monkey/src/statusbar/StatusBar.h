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
#ifndef QUEUEDSTATUSBAR_H
#define QUEUEDSTATUSBAR_H

#include <MonkeyExport.h>
#include <qsciscintilla.h>

#include <QStatusBar>

class QLabel;

class Q_MONKEY_EXPORT StatusBar : public QStatusBar
{
    Q_OBJECT
    
public:
    enum LabelType { ltCursorPosition, ltSaveState, ltEOLMode, ltIndentMode };
    StatusBar( QWidget* parent = 0 );
    
    QLabel* label( StatusBar::LabelType type );

protected:
    QLabel* mLabels[4];

public slots:
    void setMessage( const QString& message );
    void setModified( bool modified );
    void setEOLMode( QsciScintilla::EolMode mode ); // -1 for none
    void setIndentMode( int mode ); // -1 for none, 0 for spaces, 1 for tabs
    void setCursorPosition( const QPoint& pos ); // QPoint( -1, -1 ) for none
};

#endif // QUEUEDSTATUSBAR_H
