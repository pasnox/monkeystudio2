/*
*   $Id: entry.c,v 1.11 2006/05/30 04:37:12 darren Exp $
*
*   Copyright (c) 1996-2002, Darren Hiebert
*
*   This source code is released for free distribution under the terms of the
*   GNU General Public License.
*
*   This module contains functions for creating tag entries.
*/


/*
*   INCLUDE FILES
*/
#include "general.h"  /* must always come first */

#include <ctype.h>        /* to define isspace () */
#include <errno.h>
#include <stdlib.h>

/*  These header files provide for the functions necessary to do file
 *  truncation.
 */

#include "entry.h"
#include "routines.h"
#include "options.h"
#include "read.h"
#include "debug.h"

/*
*   DATA DEFINITIONS
*/
/*When scaning, items adding to the single-linked list. 
  Here is pointers on it
*/
TagEntryListItem* firstTagEntry = NULL;  /*pointer to the first entry in the list
											for return it as result of parsing file */
TagEntryListItem* lastTagEntry = NULL;   //for adding entryes


/*
*   FUNCTION DEFINITIONS
*/

void addEntryToList (const tagEntryInfo *const tag) 
{
	TagEntryListItem* newEntry = calloc ( 1,sizeof (TagEntryListItem ));
	// copying entry
	memcpy ( &newEntry->tag, tag, sizeof (tagEntryInfo));
	if ( tag->language )
	{
		newEntry->tag.language = malloc ( strlen (tag->language) + 1 );
		strcpy ( (void*)newEntry->tag.language, tag->language );
	}
	if ( tag->sourceFileName )
	{
		newEntry->tag.sourceFileName = malloc ( strlen (tag->sourceFileName) + 1 );
		strcpy ( (void*)newEntry->tag.sourceFileName, tag->sourceFileName ) ;
	}
	if ( tag->name)
	{
		newEntry->tag.name = malloc ( strlen ( tag->name ) + 1 );
		strcpy ( (void*)newEntry->tag.name, tag->name );
	}
	if ( tag->kindName)
	{
		newEntry->tag.kindName = malloc ( strlen (tag->kindName ) + 1 );
		strcpy ( (void*)newEntry->tag.kindName, tag->kindName );
	}
	if ( tag->extensionFields.access)
	{
		newEntry->tag.extensionFields.access = malloc ( strlen (tag->extensionFields.access ) + 1 );
		strcpy ( (void*)newEntry->tag.extensionFields.access, tag->extensionFields.access );
	}
	if ( tag->extensionFields.fileScope)
	{
		newEntry->tag.extensionFields.fileScope = malloc ( strlen (tag->extensionFields.fileScope ) + 1 );
		strcpy ( (void*)newEntry->tag.extensionFields.fileScope, tag->extensionFields.fileScope );
	}
	if ( tag->extensionFields.implementation)
	{
		newEntry->tag.extensionFields.implementation = malloc ( strlen (tag->extensionFields.implementation ) + 1 );
		strcpy ( (void*)newEntry->tag.extensionFields.implementation, tag->extensionFields.implementation );
	}
	if ( tag->extensionFields.inheritance)
	{
		newEntry->tag.extensionFields.inheritance = malloc ( strlen (tag->extensionFields.inheritance ) + 1 );
		strcpy ( (void*)newEntry->tag.extensionFields.inheritance, tag->extensionFields.inheritance );
	}
	if ( tag->extensionFields.scope[0])
	{
		newEntry->tag.extensionFields.scope[0] = malloc ( strlen (tag->extensionFields.scope[0] ) + 1 );
		strcpy ( (void*)newEntry->tag.extensionFields.scope[0], tag->extensionFields.scope[0] );
	}
	if ( tag->extensionFields.scope[1])
	{
		newEntry->tag.extensionFields.scope[1] = malloc ( strlen (tag->extensionFields.scope[1] ) + 1 );
		strcpy ( (void*)newEntry->tag.extensionFields.scope[1], tag->extensionFields.scope[1] );
	}
	if ( tag->extensionFields.typeRef[0])
	{
		newEntry->tag.extensionFields.typeRef[0] = malloc ( strlen (tag->extensionFields.typeRef[0] ) + 1 );
		strcpy ( (void*)newEntry->tag.extensionFields.typeRef[0], tag->extensionFields.typeRef[0] );
	}
	if ( tag->extensionFields.typeRef[1])
	{
		newEntry->tag.extensionFields.typeRef[1] = malloc ( strlen (tag->extensionFields.typeRef[1] ) + 1 );
		strcpy ( (void*)newEntry->tag.extensionFields.typeRef[1], tag->extensionFields.typeRef[1] );
	}
	if ( tag->extensionFields.signature)
	{
		newEntry->tag.extensionFields.signature = malloc ( strlen (tag->extensionFields.signature ) + 1 );
		strcpy ( (void*)newEntry->tag.extensionFields.signature, tag->extensionFields.signature );
	}
	if ( firstTagEntry == NULL )//it's will be first entry
		firstTagEntry = newEntry;
	else //it's not a first entry
		lastTagEntry->next = newEntry;
	lastTagEntry = newEntry;


}

extern void makeTagEntry (const tagEntryInfo *const tag)
{
	addEntryToList (tag) ;
}

extern void initTagEntry (tagEntryInfo *const e, const char *const name)
{
	Assert (File.source.name != NULL);
	memset (e, 0, sizeof (tagEntryInfo));
	e->lineNumberEntry = (boolean) (Option.locate == EX_LINENUM);
	e->lineNumber      = getSourceLineNumber ();
	e->language        = getSourceLanguageName ();
	e->filePosition    = getInputFilePosition ();
	e->sourceFileName  = getSourceFileTagPath ();
	e->name            = name;
}
/* vi:set tabstop=4 shiftwidth=4: */
