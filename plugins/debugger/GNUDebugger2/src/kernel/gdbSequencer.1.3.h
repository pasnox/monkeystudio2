/********************************************************************************************************
 * PROGRAM      : Debugger
 * DATE - TIME  : lundi 31 mai 2008 - 18h04
 * AUTHOR       : Xiantia
 * FILENAME     : GdbSequencer
 * LICENSE      : GPL
 * COMMENTARY   : 
 ********************************************************************************************************/

/*

	GdbSequencer class


	Xiantia@gmail.com

	for Debugger v1.3.0
*/


#ifndef GDBSEQUENCER_H
#define GDBSEQUENCER_H

#include "gdbParser.1.3.h"
#include "gdbProcess.1.3.h"

#include <QObject>
#include <QPointer>

class SequencerCmd
{

	friend class GdbSequencer;

public:
	SequencerCmd(QString n, QString c)
	{
		name = n;
		cmd = c;
	}

private:
	QString name;
	QString cmd;
};


class GdbSequencer : public QObject
{
	Q_OBJECT


public :

	GdbSequencer(QObject * parent);
	~GdbSequencer();

	void add(const SequencerCmd & );
	void add(const QString & , const QList<SequencerCmd> &);

	void start();
	void start(const int &);
	void remove();
	void remove(const SequencerCmd & );

	void loop();
	void change(const SequencerCmd &, const SequencerCmd &);

	QString currentCmd();

private :

	int mCurrentIndex; 
	QPointer<GdbParser> mParser;
	QPointer<GdbProcess> mProcess;

	QString mClassName;

	QList<SequencerCmd> mCmdList;
};

#endif
