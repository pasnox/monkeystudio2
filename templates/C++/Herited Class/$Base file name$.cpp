#include "$Base file name$.h"

$Class name$::$Class name$()
	: $Parent class$()
{
}

$Class name$::~$Class name$()
{
}

/*** For dynamic memory management (can be removed if it isn't necessary) ***/
$Class name$::$Class name$(const $Class name$ & obj)
{
}

$Class name$ & $Class name$::operator=(const $Class name$ & obj)
{
	// In order to support the operation "Obj = Obj;"
	if(*this != obj)
	{
		// Memory destruction and copy here
	}

	return *this;
}

bool $Class name$::operator ==(const $Class name$ & obj) const
{
	// Comparaison of pointer here

	return false;	// Change it
}

bool $Class name$::operator !=(const $Class name$ & obj) const
{
	return !(*this == obj);
}

/*** End of dynamic memory management ***/
