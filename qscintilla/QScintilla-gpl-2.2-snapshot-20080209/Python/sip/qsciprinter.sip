// This is the SIP interface definition for QsciPrinter.
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


class QsciPrinter : QPrinter
{
%TypeHeaderCode
#include <Qsci/qsciprinter.h>
%End

public:
    QsciPrinter(QPrinter::PrinterMode = QPrinter::ScreenResolution);
    virtual ~QsciPrinter();

    virtual void formatPage(QPainter &painter, bool drawing, QRect &area,
            int pagenr);
    int magnification() const;
    virtual void setMagnification(int magnification);
    virtual int printRange(QsciScintillaBase *qsb, int from = -1, int to = -1);
    QsciScintilla::WrapMode wrapMode() const;
    virtual void setWrapMode(QsciScintilla::WrapMode);

private:
    QsciPrinter(const QsciPrinter &);
};
