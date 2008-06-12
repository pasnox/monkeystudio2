

#include "gdbSequencer.1.3.h"


GdbSequencer::GdbSequencer(QObject * parent) : QObject(parent)
{
}


GdbSequencer::~GdbSequencer()
{
}

void GdbSequencer::add(const SequencerCmd & c)
{
	mCmdList << c;
}

void GdbSequencer::add(const QString & className, const QList<SequencerCmd> & s)
{
	mCmdList.clear();
	mCmdList = s;
	mClassName = className;
}

void GdbSequencer::start()
{
	start(0);
}

void GdbSequencer::start(const int & i)
{
	mCurrentIndex = i;

	if(mCurrentIndex < mCmdList.count())
	{
		GdbParser::instance()->setNextCommand(mClassName, mCmdList.at(i).cmd );
		GdbProcess::instance()->sendRawData( mCmdList.at(i).cmd );
	}
}

void GdbSequencer::remove()
{
	mCmdList.clear();
}

void GdbSequencer::remove(const SequencerCmd & a)
{
	for(int i = 0 ; i <mCmdList.count(); i++)
		if(mCmdList.at(i).name == a.name)
			mCmdList.removeAt(i);
}

void GdbSequencer::loop()
{
	mCurrentIndex++;
	start(mCurrentIndex);
}

void GdbSequencer::change(const SequencerCmd & a, const SequencerCmd & b )
{
	for(int i = 0 ; i <mCmdList.count(); i++)
		if(mCmdList.at(i).name == a.name)
			mCmdList.replace(i, b);
}


