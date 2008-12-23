#include "$Base File Name$.h"

$Class Name$::$Class Name$()
{
}

$Class Name$::~$Class Name$()
{
}

/*** For dynamic memory management (can be removed if it isn't necessary) ***/
$Class Name$::$Class Name$( const $Class Name$& other )
{
	// copy datas there
}

$Class Name$& $Class Name$::operator=( const $Class Name$& other )
{
	// In order to support the operation "Obj = Obj;"
	if( *this != other )
	{
		// Copy datas there
	}

	return *this;
}

bool $Class Name$::operator==( const $Class Name$& other ) const
{
	// Comparaison of datas here

	return false;	// Change it
}

bool $Class Name$::operator!=( const $Class Name$& other ) const
{
	return !operator==( other );
}
/*** End of dynamic memory management ***/
