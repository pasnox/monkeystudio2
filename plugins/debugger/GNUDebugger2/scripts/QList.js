/* 
	QList< _T >
*/



var listSize;
var elementAddr;
var elementIndex;
var res;
var mVarName;
var MAX_LEN=10;

// main function
process = function(parent , varName , isPointer , sequenceNumber)
{
	// get is varName is a pointer
	op = isPointer ? "->" : ".";
	// init
	mVarName = varName;
	res =  "{ ";
	elementIndex = 0;

	// first commant send to gdb
	// and call processSize function when all data from gdb is ready
	return "WAIT:p (" + mVarName + ")"+ op +"d.begin:processSizeBegin";
}

//
processSizeEx = function(parent , data, isPointer)
{
	// get is varName is a pointer
	op = isPointer ? "->" : ".";
	// QList finish ? 
	if(elementIndex < listSize)
		return "WAIT:p (" + mVarName + ")" + op + "d.array[" + elementIndex + "]:processElementAddr";
	else return "FINISH:" + res + " }:NULL";
}

// store data from script child ( _T nextScriptChild)
processVarFromChild = function(parent, data, isPointer)
{
	res =  res + elementIndex + " = " + data;
	if(elementIndex < listSize)
		res = res + " , ";
	
	return processSizeEx(parent, data, isPointer);
}

// extract size 
processSizeBegin = function(parent, data, isPointer)
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
		return "WAIT:p (" + mVarName + ")"+ op +"d.end:processSizeEnd";
	}			
	return "FINISH:Can't translate QList:NULL";
}

// extract size 
processSizeEnd = function(parent, data, isPointer)
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
processElementAddr= function(parent, data , isPointer)
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
