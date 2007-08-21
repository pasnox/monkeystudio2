// This is the SIP interface definition for QsciMacro.
//
// Copyright (c) 2007
// 	Riverbank Computing Limited <info@riverbankcomputing.co.uk>
// 
// This file is part of QScintilla.
// 
// This copy of QScintilla is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2, or (at your option) any
// later version.
// 
// QScintilla is supplied in the hope that it will be useful, but WITHOUT ANY
// WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
// FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more
// details.
// 
// You should have received a copy of the GNU General Public License along with
// QScintilla; see the file LICENSE.  If not, write to the Free Software
// Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.


class QsciMacro : QObject
{
%TypeHeaderCode
#include <Qsci/qscimacro.h>
%End

public:
%If (Qsci_Qt4)
    QsciMacro(QsciScintilla *parent /TransferThis/);
    QsciMacro(const QString &asc, QsciScintilla *parent /TransferThis/);
%End
%If (!Qsci_Qt4)
    QsciMacro(QsciScintilla *parent /TransferThis/, const char *name = 0);
    QsciMacro(const QString &asc, QsciScintilla *parent /TransferThis/,
            const char *name = 0);
%End

    virtual ~QsciMacro();

    void clear();
    bool load(const QString &asc);
    QString save() const;

public slots:
    virtual void play();
    virtual void startRecording();
    virtual void endRecording();

private:
    QsciMacro(const QsciMacro &);
};
