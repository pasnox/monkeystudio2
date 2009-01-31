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

ATTENTION : Monkey Studio require at least Qt 4.4.0 MINIMUM !

Before building, you should know about some rules if you are unix/linux users.
By default monkey studio will install itself in /usr.
If you want to change this you should do these build step ( assuming you want to install in /usr/local ) :

	* cd to root sources of monkey studio
	* qmake prefix=/usr/local
	* make
	* sudo make install

You now have a full working copy of monkey for your unix/linux box.
If you have trouble when monkey starting ( because you already use old version of monkey studio ), you should rename the file ~/.Monkey Studio/Monkey Studio.ini and restart monkey studio.
If trouble persists, please inform us.

How to build Monkey Studio:
===========================

With MinGW/Windows:

* Open a console, and go to the sources directory.
	* qmake && mingw32-make

With MSVC/Windows:

* Open a console, and go to the sources directory.
	* qmake && nmake // possible you will need to set, for wich compiler to generate makefile like : qmake -spec win32-msvc2005

With Linux/*Nix:

* Open a shell, and go to the sources directory.
	* qmake && make // some distros use qmake-qt4 for qmake of Qt 4

With Mac OS X:

* Open a terminal, and go to the sources directory.
	* qmake -spec macx-g++ && make

How to execute Monkey Studio:
=============================
* Once you have build monkeystudio you need install resources executing this command : $$MAKE install ( replace $$MAKE by make/mingw32-make/nmake according to your platform ).
* Once you have installed resources, you now have a full monkeystudio copy in your bin folder by default on windows/mac, and in /usr/local for unix/linux.

You may require admin acces when using $$MAKE install.
For unix-like, simply use a sudo, for windows, you need to have admin access.

Known Problems:

* MSVC 6 can't build Monkey Studio 2, and will probably never ( except if someone is able to send us a patch ).
* For any problems, please contact us on the forums at : http://forums.monkeystudio.org or on our irc channel, irc://irc.freenode.net/monkeystudio
* You can report bugs using our webissues server ( see About dialog for more infos )

How to generate Doxygen documentation:
======================================
You should execute command 
doxygen doxygen.config
from root dirrectory of source archive
(dirrectory, where config storing)
Directory 'doc/html' with documentation will appear.

	Azevedo Filipe & The Monkey Studio Team,

--EOF
