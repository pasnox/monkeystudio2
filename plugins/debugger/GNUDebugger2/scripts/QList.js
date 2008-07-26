/* 
	QList< _T >
*/



var listSize;
var elementAddr;
var elementIndex;
var res;
var mVarName;
var mCast;
var MAX_LEN=10;

// main function
process = function(parent , varName ,cast, isPointer)
{
	// get is varName is a pointer
	op = isPointer ? "->" : ".";
	// init
	mVarName = varName;
	mCast = cast;

	res =  "{ ";
	elementIndex = 0;

	Exp = new RegExp("(.*)\\s+(\\w+)");
	if(Exp.test(mVarName))
	{
		t = Exp.exec(mVarName);
		return "WAIT:p ((QList<" + mCast + ">*) " +  t[2] + ")->d.begin:processSizeBegin";

	}

	Exp = new RegExp("(\\w+)");

	if(Exp.test(mVarName))
	{
		return "WAIT:p (" + mVarName + ")"+ op +"d.begin:processSizeBegin";
	}

	return "FINISH:Can't translate QString:NULL";
}

//
processSizeEx = function(parent , data, cast ,isPointer)
{
	// get is varName is a pointer
	op = isPointer ? "->" : ".";
	// QList finish ? 
	if(elementIndex < listSize)
	{
	Exp = new RegExp("(.*)\\s+(\\w+)");
	if(Exp.test(mVarName))
	{
		t = Exp.exec(mVarName);
		return "WAIT:p ((QList<" + mCast + ">*) " +  t[2] + ")->d.array[" + elementIndex + "]:processElementAddr";

	}

	Exp = new RegExp("(\\w+)");

	if(Exp.test(mVarName))
	{
		return "WAIT:p (" + mVarName + ")"+ op +"d.array[" + elementIndex + "]:processElementAddr";
	}
//	return "WAIT:p (" + mVarName + ")" + op + "d.array[" + elementIndex + "]:processElementAddr";
	}
	else return "FINISH:" + res + " }:NULL";
}

// store data from script child ( _T nextScriptChild)
processVarFromChild = function(parent, data, cast ,  isPointer)
{
	res =  res + elementIndex + " = " + data;
	if(elementIndex < listSize)
		res = res + " , ";
	
	return processSizeEx(parent, data, isPointer);
}

// extract size 
processSizeBegin = function(parent, data, cast ,isPointer)
{
	// get is varName is a pointer
	op = isPointer ? "->" : ".";

	// $1 = 23
	ls = new RegExp("\\$\\d+\\s+=\\s+(\\d+)");
	if(ls.test(data))
	{
		t = ls.exec(data);
		// store size of QList< _T > (number of elements)
		listSize = t[1];

		Exp = new RegExp("(.*)\\s+(\\w+)");
		if(Exp.test(mVarName))
		{
			t = Exp.exec(mVarName);
			return "WAIT:p ((QList<" + mCast + ">*) " +  t[2] + ")->d.end:processSizeEnd";

		}

		Exp = new RegExp("(\\w+)");
	
		if(Exp.test(mVarName))
		{
			return "WAIT:p (" + mVarName + ")"+ op +"d.end:processSizeEnd";
		}


//		return "WAIT:p (" + mVarName + ")"+ op +"d.end:processSizeEnd";
	}			
	return "FINISH:Can't translate QList:NULL";
}

// extract size 
processSizeEnd = function(parent, data, cast , isPointer)
{
	// $1 = 23
	ls = new RegExp("\\$\\d+\\s+=\\s+(\\d+)");
	if(ls.test(data))
	{
		t = ls.exec(data);
		// store size of QList< _T > (number of elements)
		if(listSize != 0  ) 
			return "FINISH:Can't translate QList:NULL";
		listSize = t[1];
		if(listSize  > MAX_LEN) listSize = MAX_LEN;
		// call processSizeEx function
		return processSizeEx(parent, data, isPointer);
	}			
	return "FINISH:Can't translate QList:NULL";
}

// extract adress of element
processElementAddr= function(parent, data , cast , isPointer)
{
	// $2 = (void *) 0x3D4fde
//	ea = new RegExp("\\$\\d+\\s+=\\s+\\(.*\\)\\s+(\\w+)");
	ea = new RegExp("\\$\\d+\\s+=\\s+(.*)");
	if(ea.test(data))
	{
		t = ea.exec(data);
		elementAddr = t[1];
		elementIndex++; 
		// return LEAVe indicate i switch to next script ( QList< _T nexScriptChild> )
		return "LEAVE:" + elementAddr + ":processVarFromChild";
	}			
	return "FINISH:Can't translate QList:NULL";
}
