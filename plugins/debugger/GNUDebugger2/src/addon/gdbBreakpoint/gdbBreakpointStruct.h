#ifndef GDBBREAKPOINTSTRUCT
#define GDBBREAKPOINTSTRUCT

 


	typedef struct BaseBreakpoint
	{
		int line;
		int index;
		bool hit;
		int type;
		bool enable;
		QString condition;
	};


	typedef struct Breakpoint
	{
		QString fileName;
		QList<BaseBreakpoint> bp;
	};

#endif
