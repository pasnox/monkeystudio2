/****************************************************************************
**
** 		Created using Monkey Studio v1.8.0.0
** Author    : Azevedo Filipe aka Nox P@sNox <pasnox@gmail.com>, The Monkey Studio Team
** Project   : Monkey Studio 2
** FileName  : readme.txt
** Date      : 2007-11-04T22:47:00
** License   : GPL
** Comment   : Monkey Studio is a Free, Fast and lightweight crossplatform Qt RAD.
It's extendable with a powerfull plugins system.
** Home Page : http://www.monkeystudio.org
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/

How to build Monkey Studio:
===========================

With MinGW/Windows:

* Open a console, and go to the sources directory.
* type: qmake && mingw32-make release

With Linux/*Nix:

* Open a shell, and go to the sources directory.
* type: qmake && make release // some distros use qmake-qt4 for qmake of Qt 4

With Mac OS X:

* Open a shell, and go to the sources directory.
* type: qmake -macx && make release


How to execute Monkey Studio:
=============================

* Once it is built, there is a bin directoryr. Go in and execute the "monkey" binary.
* There is no install script available now. We will provide one with a second beta.

Known Problems:

* Monkey Studio can't build plugins on mac os x, if you are able to fix it, please tell us.
* For any problems, please contact us on the forums at : http://forums.monkeystudio.org

WE NEED MAC OS X USERS WITH EXPERIENCE IN Qt 4, THANKS


	Azevedo Filipe and The Monkey Studio Team,

--EOF
