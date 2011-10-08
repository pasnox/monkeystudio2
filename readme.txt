Monkey Studio IDE is an Open Source cross platform IDE written with C++/Qt4.
It's licensed in GNU GPL2 & up.
http://monkeystudio.org

NOTICE: Commercial reuse, please inform us before reusing Monkey Studio IDE.
*******

WARNING: Monkey Studio require at least Qt 4.7.0 MINIMUM !
********

Before building, you should know about some rules if you are unix/linux users.
By default monkey studio will install itself in /usr/local.
If you want to change this you should do these build step ( assuming you want to install in /usr ) :

    * cd to root sources of monkey studio
    * qmake -r prefix=/usr
    * make
    * qmake -r prefix=/usr
    * sudo make install

You now have a full working copy of monkey for your unix/linux box.
If you have trouble when monkey starting ( because you already use old version of monkey studio ), you should rename the file ~/.Monkey Studio/Monkey Studio.ini and scripts-XXX and restart monkey studio.
If trouble persists, please inform us.

How to build Monkey Studio:
===========================

With MinGW/Windows:

* Open a console, and go to the sources directory.
    * qmake -r && mingw32-make && mingw32-make install
    * The install will copy everything needed to create a stand alone copy folder of MkS.

With MSVC/Windows:

* Open a console, and go to the sources directory.
    * qmake -r && nmake && nmake install // possible you will need to set, for wich compiler to generate makefile like : qmake -spec win32-msvc2005
    * The install will copy everything needed to create a stand alone copy folder of MkS.

With Linux/*Nix:

* Open a shell, and go to the sources directory.
    * qmake -r && make // some distros use qmake-qt4 for qmake of Qt 4

With Mac OS X:

* Open a terminal, and go to the sources directory.
    * qmake -r -spec macx-g++ && make aa make install
    * The install will copy everything needed to create a stand alone copy bundle of MkS.

How to execute Monkey Studio:
=============================
* Once you have build monkeystudio you need install resources executing this command : $$MAKE install ( replace $$MAKE by make/mingw32-make/nmake according to your platform ).
* Once you have installed resources, you now have a full monkeystudio copy in your bin folder by default on windows/mac, and in /usr/local for unix/linux.

You may require admin acces when using $$MAKE install.
For unix-like, simply use a sudo, for windows, you need to have admin access.

Known Problems:

* MSVC 6 can't build Monkey Studio 2, and will probably never ( except if someone is able to send us patches ).
* For any problems, please contact us on the forums at : http://monkeystudio.org/forum or on our irc channel, irc://irc.freenode.net/monkeystudio
* You can report bugs using our webissues server or launchpad ( see About dialog for more infos )

How to generate Doxygen documentation:
======================================
You should execute command
doxygen doxygen.config
from root dirrectory of source archive
(dirrectory, where config storing)
Directory 'doc/html' with documentation will appear.

    Azevedo Filipe & The Monkey Studio Team,

--EOF
