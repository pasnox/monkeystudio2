// This is the SIP interface definition for QsciLexerIDL.
//
// Copyright (c) 2008
// 	Phil Thompson <phil@river-bank.demon.co.uk>
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


class QsciLexerIDL : QsciLexerCPP
{
%TypeHeaderCode
#include <Qsci/qscilexeridl.h>
%End

public:
%If (Qsci_Qt4)
    QsciLexerIDL(QObject *parent /TransferThis/ = 0);
%End
%If (!Qsci_Qt4)
    QsciLexerIDL(QObject *parent /TransferThis/ = 0, const char *name = 0);
%End

    virtual ~QsciLexerIDL();

    const char *language() const;
    QColor defaultColor(int style) const;
    const char *keywords(int set) const;
    QString description(int style) const;

private:
    QsciLexerIDL(const QsciLexerIDL &);
};
