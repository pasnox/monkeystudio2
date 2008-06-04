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

GdbRestoreLine::GdbRestoreLine(QObject *parent ) : QObject(parent)
{
	GdbRestoreLineList.clear();
}


GdbRestoreLine::~GdbRestoreLine()
{
	GdbRestoreLineList.clear();
}


void GdbRestoreLine::add(const QRegExp & l1, const QRegExp & l2) 
{
	GdbLines l={ l1, l2 };
	GdbRestoreLineList << l;
}

void GdbRestoreLine::add(const QString & l1, const QString & l2) 
{
	GdbLines l={ QRegExp(l1), QRegExp(l2) };
	GdbRestoreLineList << l;
}


int GdbRestoreLine::begin(const QStringList & l, const QRegExp &r)
{
	for(int i=0; i<l.count() ; i++)
		if(r.exactMatch(l.at(i))) return i;
	return -1;
}

int GdbRestoreLine::end(const int & b, const QStringList & l, const QRegExp &r)
{
	for(int i=b; i<l.count() ; i++)
		if(r.exactMatch(l.at(i))) return i;
	return -1;
}

bool GdbRestoreLine::find(const QString & l1, const QString & l2)
{
	for(int i=0; i<GdbRestoreLineList.count() ; i++)
		if(GdbRestoreLineList.at(i).l1.exactMatch(l1)  && GdbRestoreLineList.at(i).l2.exactMatch(l2)) return true;
	return false;
}

bool GdbRestoreLine::tryRestore(QStringList * list)
{
	// new version 1.4
	foreach(GdbLines l , GdbRestoreLineList)
	{
		int lBegin = begin(*list, l.l1);
		if(lBegin != -1)
		{
//			QMessageBox::warning(NULL,"l1 found", list->at(lBegin));
			int lEnd = end(lBegin, *list , l.l2);
			if(lEnd != -1)
			{
//				QMessageBox::warning(NULL,"l2 found", list->at(lEnd));
				// ok found
				QString s;
				for(int i=lBegin; i<=lEnd; i++)
				{
					// create line
					s.append( list->at(lBegin));
					list->removeAt(lBegin);
				}
				list->insert(lBegin, s);
//				QMessageBox::warning(NULL,"restoring", s);
				return true;
			}
		}
	}

	return false;	
	
/*	for(int i=0; i<list->count() - 1 ; i++)
	{
		QString a = list->at(i);
		QString b = list->at(i+1); 

		if(find( a , b))
		{
			// line splited			
			list->replace(i, a+b);
			list->removeAt(i+1);

			qDebug( "(Class GdbRestoreLine function tryRestore) WARNNING : Restoring line \"" + QString(a+b).toLocal8Bit() + "\"");
		}
	}
*/}
