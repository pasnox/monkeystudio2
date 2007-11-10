#include "options.h"
#include "string.h"
#include "routines.h"

optionValues Option = {
	{
		FALSE,  /* --extra=f */
		FALSE,  /* --extra=q */
		TRUE,   /* --file-scope */
	},
	{
		FALSE,  /* -fields=a */
		TRUE,   /* -fields=f */
		FALSE,  /* -fields=m */
		FALSE,  /* -fields=i */
		TRUE,   /* -fields=k */
		FALSE,  /* -fields=z */
		FALSE,  /* -fields=K */
		FALSE,  /* -fields=l */
		FALSE,  /* -fields=n */
		TRUE,   /* -fields=s */
		FALSE,  /* -fields=S */
		TRUE    /* -fields=t */
	},
	NULL,       /* -I */
	FALSE,      /* -a */
	FALSE,      /* -B */
	FALSE,      /* -e */
#ifdef MACROS_USE_PATTERNS
	EX_PATTERN, /* -n, --excmd */
#else
	EX_MIX,     /* -n, --excmd */
#endif
	FALSE,      /* -R */
	SO_SORTED,  /* -u, --sort */
	FALSE,      /* -V */
	FALSE,      /* -x */
	NULL,       /* -L */
	NULL,       /* -o */
	NULL,       /* -h */
	NULL,       /* --etags-include */
	0,/* --format */
	FALSE,      /* --if0 */
	FALSE,      /* --kind-long */
	LANG_AUTO,  /* --lang */
	TRUE,       /* --links */
	FALSE,      /* --filter */
	NULL,       /* --filter-terminator */
	FALSE,      /* --tag-relative */
	FALSE,      /* --totals */
	FALSE,      /* --line-directives */
	TRUE        /* library mode, generating entry list */
#ifdef DEBUG
	0, 0        /* -D, -b */
#endif
};

extern void verbose (const char *const format, ...)
{
	if (Option.verbose)
	{
		va_list ap;
		va_start (ap, format);
		vprintf (format, ap);
		va_end (ap);
	}
}

/*  Determines whether or not "name" should be ignored, per the ignore list.
 */
extern boolean isIgnoreToken (
		const char *const name, boolean *const pIgnoreParens,
		const char **const replacement)
{
	boolean result = FALSE;

	if (Option.ignore != NULL)
	{
		const size_t nameLen = strlen (name);
		unsigned int i;

		if (pIgnoreParens != NULL)
			*pIgnoreParens = FALSE;

		for (i = 0  ;  i < stringListCount (Option.ignore)  ;  ++i)
		{
			vString *token = stringListItem (Option.ignore, i);

			if (strncmp (vStringValue (token), name, nameLen) == 0)
			{
				const size_t tokenLen = vStringLength (token);

				if (nameLen == tokenLen)
				{
					result = TRUE;
					break;
				}
				else if (tokenLen == nameLen + 1  &&
						vStringChar (token, tokenLen - 1) == '+')
				{
					result = TRUE;
					if (pIgnoreParens != NULL)
						*pIgnoreParens = TRUE;
					break;
				}
				else if (vStringChar (token, nameLen) == '=')
				{
					if (replacement != NULL)
						*replacement = vStringValue (token) + nameLen + 1;
					break;
				}
			}
		}
	}
	return result;
}

/*  Determines whether the specified file name is considered to be a header
 *  file for the purposes of determining whether enclosed tags are global or
 *  static.
 */
extern boolean isIncludeFile (const char *const fileName)
{
	boolean result = FALSE;
	const char *const extension = fileExtension (fileName);
	if (Option.headerExt != NULL)
		result = stringListExtensionMatched (Option.headerExt, extension);
	return result;
}

extern void freeList (stringList** const pList)
{
	if (*pList != NULL)
	{
		stringListDelete (*pList);
		*pList = NULL;
	}
}
