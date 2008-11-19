#ifndef $Base File Name.upper$_H
#define $Base File Name.upper$_H

class $Class Name$
{
public:
	$Class Name$();
	virtual ~$Class Name$();

	/*** For dynamic memory management (can be removed if it isn't necessary) ***/
	$Class Name$( const $Class Name$& other );
	$Class Name$& operator=( const $Class Name$& other );
	bool operator==( const $Class Name$& other ) const;
	bool operator!=( const $Class Name$& other ) const;
	/*** End of dynamic memory management ***/
};

#endif // $Base File Name.upper$_H
