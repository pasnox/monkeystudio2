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

Windows/MinGW users can use batch files to build Monkey Studio.

With MSVC/Windows:

* Open a console, and go to the sources directory.
* type: qmake && nmake release

With Linux/*Nix:

* Open a shell, and go to the sources directory.
* type: qmake && make release // some distros use qmake-qt4 for qmake of Qt 4
* you can then use "cd monkey && sudo make install" for install application, 
binary will be copieds to /usr/bin and all plugins and relating resources ( templates, apis ... ) will be copied to
/usr/lib/monkeystudio

With Mac OS X:

* Open a terminal, and go to the sources directory.
* type: qmake -spec macx-g++ && make release
* you can then use "cd monkey && make install" for install all resources in the bundle.
You now have a full bundle that you can put where you want.

How to execute Monkey Studio:
=============================

* Once it is built, there is a bin directory. Go in and execute the "monkey" binary.

Known Problems:

* MSVC 6 can't build Monkey Studio 2, and will probably never ( except if someone is able to send us a patch ).
* For any problems, please contact us on the forums at : http://forums.monkeystudio.org or on our irc channel, irc://irc.freenode.net/monkeystudio


	Azevedo Filipe and The Monkey Studio Team,

--EOF
