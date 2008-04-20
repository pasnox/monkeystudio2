/* 
	QString

 Script de test de		
 decompilation des QString	
				
 par Xiantia			
				
 Pour plugin GNUDebugger	

 Get the size of string:
	
	(gdb) p s.size()
	$1 = 6
	(gdb)

 Get the char :

	(gdb) x /6s s.data()
	0x3d3fb2:        "c"
	0x3d3fb4:        "o"
	0x3d3fb6:        "u"
	0x3d3fb8:        "c"
	0x3d3fba:        "o"
	0x3d3fbc:        "u"
	(gdb)
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

var Size;


send = function(parent , varName , isPointer , sequenceNumber)
{
	// get is varName is a pointer
	op = isPointer ? "->" : ".";
	switch(sequenceNumber)
	{
		// p s.d.alloc
		case 0 : return sendToGdb(parent , "p (" + varName +")"+ op + "d.alloc");
		// p s.d.data
		case 1 : return sendToGdb(parent , "x /" + Size + "s (" + varName + ")" + op + "d.data");
	}
	return "ERROR_SEQUENCE_NUMBER";
}


receive = function(parent , data, ispointer, sequenceNumber)
{
	var res = "";

	switch(sequenceNumber)
	{
		case 0 : 
		{
			ExpSize = new RegExp("\\$\\d+\\s*=\\s*(\\d+)");
			if(ExpSize.test(data))
			{
				t = ExpSize.exec(data);
				Size = t[1];
				// i return WAITING , because i have other command (force re-call QString_send function)
				return "WAIT_NEXT_COMMAND"; 
			}
			else return "ERROR_PARSE";
		}

		case 1 :
		{
//			ExpVar = new RegExp("0x\\w+:\\s+\"(.+)\"");
			ExpVar = new RegExp("0x\\w+:\\s*(.+)");

			list = data.split('\n');

			for(i=0; i<Size; i++)
			{
				if(ExpVar.test(data)) 
				{
					t = ExpVar.exec(list[i]); 
					res = res + t[1];
				}
				else return "ERROR_PARSE";
			}
	
			return  "QString(" + res + ")";
		}
	}
	return "ERROR_SEQUENCE_NUMBER";
}
