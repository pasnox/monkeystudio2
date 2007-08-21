// This is the SIP interface definition for QsciLexerHTML.
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


class QsciLexerHTML : QsciLexer
{
%TypeHeaderCode
#include <Qsci/qscilexerhtml.h>
%End

public:
    enum {
        Default,
        Tag,
        UnknownTag,
        Attribute,
        UnknownAttribute,
        HTMLNumber,
        HTMLDoubleQuotedString,
        HTMLSingleQuotedString,
        OtherInTag,
        HTMLComment,
        Entity,
        XMLTagEnd,
        XMLStart,
        XMLEnd,
        Script,
        ASPAtStart,
        ASPStart,
        CDATA,
        PHPStart,
        HTMLValue,
        ASPXCComment,
        SGMLDefault,
        SGMLCommand,
        SGMLParameter,
        SGMLDoubleQuotedString,
        SGMLSingleQuotedString,
        SGMLError,
        SGMLSpecial,
        SGMLEntity,
        SGMLComment,
        SGMLParameterComment,
        SGMLBlockDefault,
        JavaScriptStart,
        JavaScriptDefault,
        JavaScriptComment,
        JavaScriptCommentLine,
        JavaScriptCommentDoc,
        JavaScriptNumber,
        JavaScriptWord,
        JavaScriptKeyword,
        JavaScriptDoubleQuotedString,
        JavaScriptSingleQuotedString,
        JavaScriptSymbol,
        JavaScriptUnclosedString,
        JavaScriptRegex,
        ASPJavaScriptStart,
        ASPJavaScriptDefault,
        ASPJavaScriptComment,
        ASPJavaScriptCommentLine,
        ASPJavaScriptCommentDoc,
        ASPJavaScriptNumber,
        ASPJavaScriptWord,
        ASPJavaScriptKeyword,
        ASPJavaScriptDoubleQuotedString,
        ASPJavaScriptSingleQuotedString,
        ASPJavaScriptSymbol,
        ASPJavaScriptUnclosedString,
        ASPJavaScriptRegex,
        VBScriptStart,
        VBScriptDefault,
        VBScriptComment,
        VBScriptNumber,
        VBScriptKeyword,
        VBScriptString,
        VBScriptIdentifier,
        VBScriptUnclosedString,
        ASPVBScriptStart,
        ASPVBScriptDefault,
        ASPVBScriptComment,
        ASPVBScriptNumber,
        ASPVBScriptKeyword,
        ASPVBScriptString,
        ASPVBScriptIdentifier,
        ASPVBScriptUnclosedString,
        PythonStart,
        PythonDefault,
        PythonComment,
        PythonNumber,
        PythonDoubleQuotedString,
        PythonSingleQuotedString,
        PythonKeyword,
        PythonTripleSingleQuotedString,
        PythonTripleDoubleQuotedString,
        PythonClassName,
        PythonFunctionMethodName,
        PythonOperator,
        PythonIdentifier,
        ASPPythonStart,
        ASPPythonDefault,
        ASPPythonComment,
        ASPPythonNumber,
        ASPPythonDoubleQuotedString,
        ASPPythonSingleQuotedString,
        ASPPythonKeyword,
        ASPPythonTripleSingleQuotedString,
        ASPPythonTripleDoubleQuotedString,
        ASPPythonClassName,
        ASPPythonFunctionMethodName,
        ASPPythonOperator,
        ASPPythonIdentifier,
        PHPDefault,
        PHPDoubleQuotedString,
        PHPSingleQuotedString,
        PHPKeyword,
        PHPNumber,
        PHPVariable,
        PHPComment,
        PHPCommentLine,
        PHPDoubleQuotedVariable,
        PHPOperator
    };

%If (Qsci_Qt4)
    QsciLexerHTML(QObject *parent /TransferThis/ = 0);
%End
%If (!Qsci_Qt4)
    QsciLexerHTML(QObject *parent /TransferThis/ = 0, const char *name = 0);
%End

    virtual ~QsciLexerHTML();

    const char *language() const;
    const char *lexer() const;
    QColor defaultColor(int style) const;
    bool defaultEolFill(int style) const;
    QFont defaultFont(int style) const;
    QColor defaultPaper(int style) const;
    const char *keywords(int set) const;
    QString description(int style) const;

    void refreshProperties();
    bool foldCompact() const;
    bool foldPreprocessor() const;
    bool caseSensitiveTags() const;

public slots:
    virtual void setFoldCompact(bool fold);
    virtual void setFoldPreprocessor(bool fold);
    virtual void setCaseSensitiveTags(bool sens);

protected:
    bool readProperties(QSettings &qs, const QString &prefix);
    bool writeProperties(QSettings &qs, const QString &prefix) const;

private:
    QsciLexerHTML(const QsciLexerHTML &);
};
