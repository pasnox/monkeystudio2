
var mSize;
var mVarName;
var res;
var mOp;
var mCast;

process = function(parent , varName , cast , isPointer)
{
	mVarName = varName;
	res = "";
	// get is varName is a pointer
	mOp = isPointer ? "->" : ".";
	mCast = "(" + cast + ")";

	Exp = new RegExp("(.*)\\s+(\\w+)");
	if(Exp.test(mVarName))
	{
		t = Exp.exec(mVarName);
		mVarName = t[2];
		return "WAIT:p (" + mCast +  t[2] + ")" + mOp + "d.alloc:sizeFunction";
	}
	else 
		// QString seul
		return "WAIT:p (" + mCast + mVarName + ")" + mOp + "d.alloc:sizeFunction";
}


sizeFunction = function(parent, data,cast , isPointer)
{

	ExpSize = new RegExp("\\$\\d+\\s*=\\s*(\\d+)");
	if(ExpSize.test(data))
	{
		t = ExpSize.exec(data);
		mSize = t[1];
		return "WAIT:x /" + mSize + "s (" + mCast + mVarName + ")" + mOp + "d.data:stringFunction"; 
	}
	return "FINISH:Can't translate QString:NULL";
}

stringFunction = function(parent, data, cast  ,isPointer)
{
	ExpVar = new RegExp("0x\\w+:\\s*(.+)");
	list = data.split('\n');

	for(i=0; i<mSize; i++)
	{
		if(ExpVar.test(data)) 
		{
			t = ExpVar.exec(list[i]); 
			res = res + t[1];
		}
		else 	return "FINISH:Can't translate QString:NULL";
	}
	return  "FINISH:" + res + ":NULL";
}

