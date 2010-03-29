#ifndef QCTAGSSENSEGLOBALS_H
#define QCTAGSSENSEGLOBALS_H

#include "qCtagsSenseExport.h"

#include <QDir>
#include <QIcon>

namespace qCtagsSense
{
struct Entry;

	enum Access
	{
		Public = 0,
		Protected,
		Private
	};
	
	enum Language // Ordered by ctags parsers list.
	{
		UnknowLanguage = -1,
		Ant, // 0
		Assembler,
		Asp,
		Awk,
		Basic,
		Beta, // 5
		C,
		Cpp,
		CSharp,
		Cobol,
		DosBatch, // 10
		Eiffel,
		Erlang,
		Flex,
		Fortran,
		HTML, // 15
		Java,
		JavaScript,
		Lisp,
		Lua,
		Make, // 20
		MatLab,
		OCaml,
		Pascal,
		Perl,
		PHP, // 25
		Python,
		REXX,
		Ruby,
		Scheme,
		Sh, // 30
		SLang,
		SML,
		SQL,
		Tcl,
		Tex, // 35
		Vera,
		Verilog,
		VHDL,
		Vim,
		YACC
	};
	
	/*
	enum Kind
	{
		UnknowKind = -1,
		// A
		AllPatterns, // BETA
		Anchor, // HTML
		AutoCommandGroup, // Vim
		// B
		BlockData, // Fortran
		BlockCommon, // Fortran
		// C
		Class, // Vera, Tcl, Ruby, Python, PHP, JavaScript, Java, Eiffel, C#, C, C++
		Constant, // Verilog, PHP, Perl, Basic, Asp
		Cursor, // SQL
		// D
		Define, // Asm
		DerivatedType, // Fortran
		Domain, // SQL
		// E
		EntryPoint, // Fortran
		Enumeration, // Vera, Java, C#, C, C++, Basic
		Enumerator, // Vera, C#, C, C++
		EnumType, // Java
		Event, // Verilog, SQL, C#
		Exception, // SML
		// F
		Feature, // Eiffel
		Field, // Java, C#, SQL
		FileDescriptor, // Cobol
		Format, // Perl
		Fragment, // BETA
		Function, // Vim, Verilog, Vera, SQL, SML, SLang, Sh, Scheme, Python, PHP, Pascal, Lua, Lisp, JavaScript, Fortran, Erlang, C, C++, Basic, Awk, Asp
		Functor, // SML
		// G
		// H
		// I
		Index, // SQL
		Interface, // PHP, Java, Fortran, C#
		ItemData, // Cobol
		ItemGroup, // Cobol
		// J
		JavaScriptFunction, // PHP, HTML
		// K
		// L
		Label, // YACC, SQL, Perl, Fortran, Basic, Asm
		// M
		Macro, // Vera, Make, Erlang, C#, C, C++, Asm
		Map, // Vim
		Member, // Vera, Python, C, C++
		Method, // Tcl, Ruby, JavaScript, Java, C#
		MethodSingleton, // Ruby
		MobiLinkTableScript, // SQL
		MobiLinkConnScript, // SQL
		Module, // Verilog, Ruby, Fortran, Erlang
		// N
		NameList, // Fortran
		Namespace, // SLang, C#, C, C++
		NetDataType, // Verilog
		// O
		// P
		Package, // SQL, Perl, Java
		Paragraph, // Cobol
		Pattern, // BETA
		Port, // Verilog
		Procedure, // Tcl, SQL, Pascal
		Program, // Vera, Fortran
		ProgramId, // Cobol
		Property, // C#
		Prototype, // Vera, SQL, C, C++
		Publication, // SQL
		// Q
		// R
		Record, // SQL, Erlang
		RegisterDataType, // Verilog
		// S
		Section, // Cobol
		Service, // SQL
		Set, // Scheme
		Signature, // SML
		Slot, // BETA
		SubRoutine, // REXX, Perl, Fortran, Asp
		SubRoutineDeclaration, // Perl
		SubType, // SQL
		Structure, // SML, C#, C, C++
		Synonym, // SQL
		// T
		Table, // SQL
		Task, // Verilog, Vera
		Trigger, // SQL
		Type, // SML, Fortran, Basic, Asm
		Typedef, // Vera, C#, C, C++
		// U
		Union, // C, C++
		UserDefinedCommand, // Vim
		// V
		ValueBinding, // SML
		Variable, // Vim, Vera, SQL, PHP, Fortran, C, C++, Basic, Asp
		VariableLocal, // Vera, SQL, Java, Fortran, Eiffel, C#, C, C++
		VariableGlobal, // JavaScript, Vera, C, C++
		View, // SQL
		// W
		// X
		// Y
		// Z
	};
	*/
	
	QCTAGSSENSE_EXPORT void initialize();
	
	QCTAGSSENSE_EXPORT QString languageName( qCtagsSense::Language language );
	QCTAGSSENSE_EXPORT qCtagsSense::Language language( const QString& language );
	
	QCTAGSSENSE_EXPORT QString kindName( qCtagsSense::Language language, int kind );
	QCTAGSSENSE_EXPORT int kindLetter( qCtagsSense::Language language, const QString& kind );
	QCTAGSSENSE_EXPORT QString kindDescription( qCtagsSense::Language language, int kind );
	
	QCTAGSSENSE_EXPORT bool caseInsensitiveFilePathLessThan( const QString& left, const QString& right );
	QCTAGSSENSE_EXPORT QFileInfoList getFiles( QDir& fromDir, const QStringList& filters, bool recursive = true );
	
	QCTAGSSENSE_EXPORT QString entryDecorationFileName( const qCtagsSense::Entry* entry );
	QCTAGSSENSE_EXPORT QPixmap entryDecorationPixmap( const qCtagsSense::Entry* entry );
	QCTAGSSENSE_EXPORT QIcon entryDecorationIcon( const qCtagsSense::Entry* entry );
	QCTAGSSENSE_EXPORT QString entryDisplay( const qCtagsSense::Entry* entry );
	QCTAGSSENSE_EXPORT QString entryToolTip( const qCtagsSense::Entry* entry );
}; // namespace qCtagsSense

#endif // QCTAGSSENSEGLOBALS_H
