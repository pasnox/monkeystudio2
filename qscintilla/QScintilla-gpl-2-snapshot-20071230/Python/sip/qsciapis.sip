// This is the SIP interface definition for QsciAPIs.
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


class QsciAPIs : QObject
{
%TypeHeaderCode
#include <Qsci/qsciapis.h>
%End

public:
%If (Qsci_Qt4)
    QsciAPIs(QsciLexer *lexer /TransferThis/ = 0);
%End
%If (!Qsci_Qt4)
    QsciAPIs(QsciLexer *lexer /TransferThis/ = 0, const char *name = 0);
%End

    virtual ~QsciAPIs();

    void add(const QString &entry);
    void clear();
    bool load(const QString &fname);
    void remove(const QString &entry);
    void prepare();
    void cancelPreparation();
    QString defaultPreparedName() const;
    bool isPrepared(const QString &fname = QString()) const;
    bool loadPrepared(const QString &fname = QString());
    bool savePrepared(const QString &fname = QString()) const;
    virtual bool event(QEvent *e);
    QStringList installedAPIFiles() const;

signals:
    void apiPreparationCancelled();
    void apiPreparationStarted();
    void apiPreparationFinished();

private:
    QsciAPIs(const QsciAPIs &);
};
