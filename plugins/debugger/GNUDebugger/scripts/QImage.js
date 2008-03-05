/*
	QImage
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

var Width;
var Heigth;

send = function(parent , varName , isPointer , sequenceNumber)
{
	// get is varName is a pointer
	op = isPointer ? "->" : ".";

	switch(sequenceNumber)
	{
		case 0 : return sendToGdb(parent , "p (" + varName + ")" + op + "size()");
	}
	return "ERROR_SEQUENCE_NUMBER";
}

receive = function(parent , data, ispointer, sequenceNumber)
{
	var res = "";
	ExpSize = new 	RegExp("\\$\\d+\\s+=\\s*\\{\\s*wd\\s*=\\s*(.*)\\s*,\\s*ht\\s*=\\s*(.*)\\s*\\}");

	switch(sequenceNumber)
	{
		case 0 : 
		{
			t = ExpSize.exec(data);
			Width = t[1];
			Heigth= t[2];

			return "QImage(" + Width  + "," + Heigth + ")";
		}
	}
	return "ERROR_SEQUENCE_NUMBER";
}
