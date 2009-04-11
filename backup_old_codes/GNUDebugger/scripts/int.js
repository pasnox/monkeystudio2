

process = function(parent , varName , cast ,isPointer)
{
	mVarName = varName;
	// get is varName is a pointer

	Exp = new RegExp("(.*)\\s+(\\w+)");
	if(Exp.test(mVarName))
	{
		t = Exp.exec(mVarName);
		mVarName = t[2];
		return "FINISH:"+ t[2] + ":NULL";

	}
	Exp = new RegExp("(\\w+)");

	if(Exp.test(mVarName))
	{
		return "WAIT:p " + varName + ":processValue";
	}

	return "FINISH:Can't translate QString:NULL";
}	

processValue = function(parent, data, cast , isPointer)
{
	Exp = new RegExp("\\$\\d+\\s+=\\s(\\w+)");

	if(Exp.test(data))
	{
		t = Exp.exec(data);
		return "FINISH:" + t[1] + ":NULL";
	}
	return "FINISH:Can't translate QString:NULL";
}



