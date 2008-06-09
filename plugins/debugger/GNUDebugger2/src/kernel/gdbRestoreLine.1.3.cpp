/********************************************************************************************************
	* PROGRAM      : Debugger (PARSER)
	* DATE - TIME  : mardi 01 janvier 2008 - 18h48
	* AUTHOR       :  (  )
	* FILENAME     : GdbRestoreLine
	* LICENSE      : 
	* COMMENTARY   : 
	********************************************************************************************************/



#include "gdbRestoreLine.1.3.h"
#include <QMessageBox>

//

GdbRestoreLine::GdbRestoreLine(QObject *parent ) : QObject(parent)
{
	GdbRestoreLineList.clear();
}

//

GdbRestoreLine::~GdbRestoreLine()
{
	GdbRestoreLineList.clear();
}

//

void GdbRestoreLine::add(const QRegExp & l1, const QRegExp & l2) 
{
	GdbLines l={ l1, l2 };
	GdbRestoreLineList << l;
}

//

void GdbRestoreLine::add(const QString & l1, const QString & l2) 
{
	GdbLines l={ QRegExp(l1), QRegExp(l2) };
	GdbRestoreLineList << l;
}

//

int GdbRestoreLine::begin(const int & b , const QStringList & l, const QRegExp &r)
{
	for(int i=b; i<l.count() ; i++)
		if(r.exactMatch(l.at(i))) return i;
	return -1;
}

//

int GdbRestoreLine::end(const int & b, const QStringList & l, const QRegExp &r)
{
	for(int i=b; i<l.count() ; i++)
		if(r.exactMatch(l.at(i))) return i;
	return -1;
}

// not use

bool GdbRestoreLine::find(const QString & l1, const QString & l2)
{
	for(int i=0; i<GdbRestoreLineList.count() ; i++)
		if(GdbRestoreLineList.at(i).l1.exactMatch(l1)  && GdbRestoreLineList.at(i).l2.exactMatch(l2)) return true;
	return false;
}

//
/*
#0  qMain (argc=1, argv=0x3d4c30)
 at src/main.cpp:65
#1  0x004578d4 in WinMain (instance=0x400000, prevInstance=0x0, cmdShow=10)
    at qtmain_win.cpp:140
#2  0x0045730a in main ()
(gdb) 
*/
bool GdbRestoreLine::tryRestore(QStringList * list)
{
	bool r = false;

	// new version 1.4
	foreach(GdbLines l , GdbRestoreLineList)
	{
		for(int i=0; i< list->count(); i++)
		{
			int lBegin = begin(i, *list, l.l1);
			if(lBegin != -1)
			{
				int lEnd = end(lBegin, *list , l.l2);
				if(lEnd != -1)
				{
					r = true;
					// ok found
					QString s;
					for(int j=lBegin; j<=lEnd; j++)
					{
						// create line
						s.append( list->at(lBegin));
						list->removeAt(lBegin);
					}
					list->insert(lBegin, s);
					//				return true;
				}
			}
		}
	}
	return r;	
}
