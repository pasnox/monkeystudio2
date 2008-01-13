// This is the SIP interface definition for the parts of the Qsci module common
// to the Qt v3 and v4 versions.
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


%Feature Qsci_Qt4


%Copying
Copyright (c) 2008
	Phil Thompson <phil@river-bank.demon.co.uk>

This file is part of QScintilla.

This copy of QScintilla is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2, or (at your option) any
later version.

QScintilla is supplied in the hope that it will be useful, but WITHOUT ANY
WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more
details.

You should have received a copy of the GNU General Public License along with
QScintilla; see the file LICENSE.  If not, write to the Free Software
Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
%End


const int QSCINTILLA_VERSION;
const char *QSCINTILLA_VERSION_STR;


%Include qsciscintilla.sip
%Include qsciapis.sip
%Include qscicommand.sip
%Include qscicommandset.sip
%Include qscidocument.sip
%Include qscilexer.sip
%Include qscilexerbash.sip
%Include qscilexerbatch.sip
%Include qscilexercmake.sip
%Include qscilexercpp.sip
%Include qscilexercsharp.sip
%Include qscilexercss.sip
%Include qscilexerd.sip
%Include qscilexerdiff.sip
%Include qscilexerhtml.sip
%Include qscilexeridl.sip
%Include qscilexerjava.sip
%Include qscilexerjavascript.sip
%Include qscilexerlua.sip
%Include qscilexermakefile.sip
%Include qscilexerperl.sip
%Include qscilexerpov.sip
%Include qscilexerproperties.sip
%Include qscilexerpython.sip
%Include qscilexerruby.sip
%Include qscilexersql.sip
%Include qscilexertex.sip
%Include qscilexervhdl.sip
%Include qscimacro.sip
%Include qsciprinter.sip
