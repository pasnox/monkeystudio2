

#ifndef GDBBREAKPOINTSTRUCT
#define GDBBREAKPOINTSTRUCT

	//! It is a base struct for store some informations for one breakpoint

	/**
	\author xiantia
	\version 1.3.2
	
	Base Struct for one breakpoint 
	*/
	typedef struct BaseBreakpoint
	{
		//! Contains the line from editor, the breakpoint is located at this line
		int line;
		//! Contains the index from Gdb, number returned by Gdb
		int index;
		//! If index is true, it indicate that Gdb has breaked
		bool hit;
		//! Breakpoint is  : 0 = unconditionned, 1 = contidionned
		int type;
		//! Breakpoint is enable if enable is true
		bool enable;
		//! Condition for this breakpoint, for example "i==3". Gdb break only if condition is true
		QString condition;
	};

	//! It is a struct for store all brekpoints for one file

	/**
	\author xiantia
	\version 1.3.2

	Struct of all Breakpoints strored for one file
	\note One file can contains more breakpoints.
	*/
	typedef struct Breakpoint
	{
		//! Contains the fileName 
		QString fileName;
		//! Contains all breakpoints informations (list)
		QList<BaseBreakpoint> bp;
	};

#endif
