// This is the SIP interface definition for QsciLexerCPP.
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


class QsciLexerCPP : QsciLexer
{
%TypeHeaderCode
#include <Qsci/qscilexercpp.h>
%End

public:
    enum {
        Default,
        Comment,
        CommentLine,
        CommentDoc,
        Number,
        Keyword,
        DoubleQuotedString,
        SingleQuotedString,
        UUID,
        PreProcessor,
        Operator,
        Identifier,
        UnclosedString,
        VerbatimString,
        Regex,
        CommentLineDoc,
        KeywordSet2,
        CommentDocKeyword,
        CommentDocKeywordError,
        GlobalClass,
    };

%If (Qsci_Qt4)
    QsciLexerCPP(QObject *parent /TransferThis/ = 0,
            bool caseInsensitiveKeywords = false);
%End
%If (!Qsci_Qt4)
    QsciLexerCPP(QObject *parent /TransferThis/ = 0, const char *name = 0,
            bool caseInsensitiveKeywords = false);
%End

    const char *language() const;
    const char *lexer() const;
    QColor defaultColor(int style) const;
    bool defaultEolFill(int style) const;
    QFont defaultFont(int style) const;
    QColor defaultPaper(int style) const;
    const char *keywords(int set) const;
    QString description(int style) const;

    void refreshProperties();
    bool foldAtElse() const;
    bool foldComments() const;
    bool foldCompact() const;
    bool foldPreprocessor() const;
    bool stylePreprocessor() const;

public slots:
    virtual void setFoldAtElse(bool fold);
    virtual void setFoldComments(bool fold);
    virtual void setFoldCompact(bool fold);
    virtual void setFoldPreprocessor(bool fold);
    virtual void setStylePreprocessor(bool style);

protected:
    bool readProperties(QSettings &qs, const QString &prefix);
    bool writeProperties(QSettings &qs, const QString &prefix) const;

private:
    QsciLexerCPP(const QsciLexerCPP &);
};
