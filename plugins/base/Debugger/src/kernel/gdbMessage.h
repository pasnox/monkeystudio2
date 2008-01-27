
struct base_message
{
	QByteArray msg;
	int id;
};

typedef base_message QGdbMessage;

class gdb_base_message
{
private :
		QList<QGdbMessage> mMsgList;

public : 
		gdb_base_message(){};
		~gdb_base_message(){};


		 void postGdbMessage(int id , QByteArray data)
		{
			QGdbMessage m;
			m.msg = data;
			m.id = id;
			mMsgList << m;
		}

		 QGdbMessage getGdbMessage()
		{
			QGdbMessage m;
			m.id = -2;

			if(mMsgList.count())
			{
				m = mMsgList.at(0);
				mMsgList.removeAt(0);
			 }
			return m;
		}

		int getGdbMessageCount(){ return mMsgList.count();}
};

typedef gdb_base_message QGdbMessageQueu;
