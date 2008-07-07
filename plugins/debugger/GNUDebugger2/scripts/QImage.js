
var mVarName;
var mOp;
var mCast;
var Width;
var Heigth;

process = function(parent , varName , isPointer)
{
	mVarName = varName;
	// get is varName is a pointer
	mOp = isPointer ? "->" : ".";
	mCast = isPointer ? "(QString*)" : "(QString)";
	return "WAIT:p (" + mVarName + ")" + mOp + "size():sizeFunction";
}


sizeFunction = function(parent, data, isPointer)
{
	ExpSize = new 	RegExp("\\$\\d+\\s+=\\s*\\{\\s*wd\\s*=\\s*(.*)\\s*,\\s*ht\\s*=\\s*(.*)\\s*\\}");

	if(ExpSize.test(data))
	{
		t = ExpSize.exec(data);
		Width = t[1];
		Heigth= t[2];

		return "FINISH:{" + Width  + " , " + Heigth + "}:NULL";
	}
	return "FINISH:Can't translate QImage:NULL";
}


