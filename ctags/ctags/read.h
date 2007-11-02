#ifndef  _READ_H
#define  _READ_H

#include <stdio.h>
#include "vstring.h"
#include "parse.h"

#define getInputLineNumber()     File.lineNumber
#define getInputFileName()       vStringValue (File.source.name)
#define getInputFilePosition()   File.filePosition
#define getSourceFileName()      vStringValue (File.source.name)
#define getSourceFileTagPath()   File.source.tagPath
#define getSourceLanguage()      File.source.language
#define getSourceLanguageName()  getLanguageName (File.source.language)
#define getSourceLineNumber()    File.source.lineNumber
#define isLanguage(lang)         (boolean)((lang) == File.source.language)
#define isHeaderFile()           File.source.isHeader


typedef struct sInputFile {
	vString    *name;          /* name of input file */
	vString    *path;          /* path of input file (if any) */
	vString    *line;          /* last line read from file */
	const unsigned char* currentLine;  /* current line being worked on */
	FILE       *fp;            /* stream used for reading the file */
	unsigned long lineNumber;  /* line number in the input file */
	fpos_t      filePosition;  /* file position of current line */
	int         ungetch;       /* a single character that was ungotten */
	boolean     eof;           /* have we reached the end of file? */
	boolean     newLine;       /* will the next character begin a new line? */
	langType    language;      /* language of input file */

	/*  Contains data pertaining to the original source file in which the tag
	 *  was defined. This may be different from the input file when #line
	 *  directives are processed (i.e. the input file is preprocessor output).
	 */
	struct sSource {
		vString *name;           /* name to report for source file */
		char    *tagPath;        /* path of source file relative to tag file */
		unsigned long lineNumber;/* line number in the source file */
		boolean  isHeader;       /* is source file a header file? */
		langType language;       /* language of source file */
	} source;
} inputFile;

enum eCharacters {
	/* white space characters */
	SPACE         = ' ',
	NEWLINE       = '\n',
	CRETURN       = '\r',
	FORMFEED      = '\f',
	TAB           = '\t',
	VTAB          = '\v',

	/* some hard to read characters */
	DOUBLE_QUOTE  = '"',
	SINGLE_QUOTE  = '\'',
	BACKSLASH     = '\\',

	STRING_SYMBOL = ('S' + 0x80),
	CHAR_SYMBOL   = ('C' + 0x80)
};

/*
*   GLOBAL VARIABLES
*/
extern  inputFile File;





extern const unsigned char *fileReadLine (void);
extern int fileGetc (void);
extern void fileUngetc (int c);
extern char *readLine (vString *const vLine, FILE *const fp);
extern boolean fileOpen (const char *const fileName, const langType language);

#endif  /* _READ_H */
