// This is the SIP interface definition for QsciLexerTeX.
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


class QsciLexerTeX : QsciLexer
{
%TypeHeaderCode
#include <Qsci/qscilexertex.h>
%End

public:
    enum {
        Default,
        Special,
        Group,
        Symbol,
        Command,
        Text
    };

%If (Qsci_Qt4)
    QsciLexerTeX(QObject *parent /TransferThis/ = 0);
%End
%If (!Qsci_Qt4)
    QsciLexerTeX(QObject *parent /TransferThis/ = 0, const char *name = 0);
%End

    virtual ~QsciLexerTeX();

    const char *language() const;
    const char *lexer() const;
    QColor defaultColor(int style) const;
    const char *keywords(int set) const;
    QString description(int style) const;

private:
    QsciLexerTeX(const QsciLexerTeX &);
};
