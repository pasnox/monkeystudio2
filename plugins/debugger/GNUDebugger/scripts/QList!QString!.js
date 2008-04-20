/* 
	QList<QString>
*/



/*

	When the script is call for the first time, i call QString_send function with sequenceNumber = 0.
	This send to Gdb "p " + varName + op + "size()" string.

	When i receive answer from gdb i call QString_receive function (sequenceNumber = 0).
	I parse this for extract size of QString and store this in Size var.

	After i re-call QString_send function for send to gdb "x /" + Size + " " + varName + op + "data()" 
	because now sequenceNumber = 1.

	I receive answer from gdb, parse this and create new decompiled string. 
	Script is termined because QString_receive not return "Waiting" or "TERMINED"
		
*/

var listSize;
var listAddr
var elementSize;
var elementAddr;
var elementIndex = 0;
var res="";

send = function(parent , varName , isPointer , sequenceNumber)
{
	// get is varName is a pointer
	op = isPointer ? "->" : ".";

	switch(sequenceNumber)
	{
		// p s.d.end , nombre d'element dans la list
		case 0 : return sendToGdb(parent , "p (" + varName + ")"+ op +"d.end");

		// p &s.d.array , adresse main des elements
		case 1 : return sendToGdb(parent , "p (" + varName + ")" + op + "d.array[" + elementIndex + "]");

		// p ((QString) addrElements).d.alloc , size d'un element de la list
		case 2 : return sendToGdb(parent, "p ((QString)" + elementAddr + ").d.alloc");

		// x /sizeElement(s) addrElements , string de l'element
		case 3 : return sendToGdb(parent, "x /" + elementSize + "s " + "((QString)" + elementAddr + ").d.data");
	}
	return "ERROR_SEQUENCE_NUMBER_SEND";
}


receive = function(parent , data, ispointer, sequenceNumber)
{

	switch(sequenceNumber)
	{
		case 0 : // get size of list
		{
			// $1 = 23
			ls = new RegExp("\\$\\d+\\s+=\\s+(\\d+)");
			if(ls.test(data))
			{
				t = ls.exec(data);
				listSize = t[1];
				return "WAIT_NEXT_COMMAND";
			}			
			return "ERROR_PARSE";
		}

		case 1 : // get Addr of element
		{
			// $2 = (void *) 0x3D4fde
			ea = new RegExp("\\$\\d+\\s+=\\s+\\(.*\\)\\s+(\\w+)");
			if(ea.test(data))
			{
				t = ea.exec(data);
				elementAddr = t[1];
				return "WAIT_NEXT_COMMAND";
			}			
			return "ERROR_PARSE";
		}

		case 2 : // get size of element
		{
			// $4 = 6
			es = new RegExp("\\$\\d+\\s+=\\s(\\d+)");
			if(es.test(data))
			{
				t = es.exec(data);
				elementSize = t[1];
				return "WAIT_NEXT_COMMAND";
			}			
			return "ERROR_PARSE";
		}

		case 3 :
		{
			ExpVar = new RegExp("0x\\w+:\\s*(.+)");

			list = data.split('\n');

			for(i=0; i<elementSize; i++)
			{
				if(ExpVar.test(data)) 
				{
					t = ExpVar.exec(list[i]); 
					res = res + t[1];
				}
				else return "ERROR_PARSE";
			}
			elementIndex++;

			if(elementIndex	== listSize)
				return "QList(" + res + ")";
			else
			{
				res = res + " , ";
				repeatSequenceAt(parent , 1);
				return "WAIT_NEXT_COMMAND";
			}
			
		}

	}
	return "ERROR_SEQUENCE_NUMBER_RECEIVE";
}
