// This is the SIP interface definition for QsciLexerCSS.
//
// Copyright (c) 2007
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


class QsciLexerCSS : QsciLexer
{
%TypeHeaderCode
#include <Qsci/qscilexercss.h>
%End

public:
    enum {
        Default,
        Tag,
        ClassSelector,
        PseudoClass,
        UnknownPseudoClass,
        Operator,
        CSS1Property,
        UnknownProperty,
        Value,
        Comment,
        IDSelector,
        Important,
        AtRule,
        DoubleQuotedString,
        SingleQuotedString,
        CSS2Property,
        Attribute
    };

%If (Qsci_Qt4)
    QsciLexerCSS(QObject *parent /TransferThis/ = 0);
%End
%If (!Qsci_Qt4)
    QsciLexerCSS(QObject *parent /TransferThis/ = 0, const char *name = 0);
%End

    virtual ~QsciLexerCSS();

    const char *language() const;
    const char *lexer() const;
    QColor defaultColor(int style) const;
    QFont defaultFont(int style) const;
    const char *keywords(int set) const;
    QString description(int style) const;

    void refreshProperties();
    bool foldComments() const;
    bool foldCompact() const;

public slots:
    virtual void setFoldComments(bool fold);
    virtual void setFoldCompact(bool fold);

protected:
    bool readProperties(QSettings &qs, const QString &prefix);
    bool writeProperties(QSettings &qs, const QString &prefix) const;

private:
    QsciLexerCSS(const QsciLexerCSS &);
};
