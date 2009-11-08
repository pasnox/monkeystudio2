#include "CompilerPlugin.h"

CompilerPlugin::CompilerPlugin()
	: BasePlugin(), CLIToolPlugin( this )
{
}

pCommand CompilerPlugin::compileCommand() const
{
	// get settings object
	pSettings* settings = MonkeyCore::settings();
	pCommand cmd;
	
	cmd.setText( settings->value( settingsKey( "CompileCommand/Text" ) ).toString() );
	cmd.setCommand( settings->value( settingsKey( "CompileCommand/Command" ) ).toString() );
	cmd.setArguments( settings->value( settingsKey( "CompileCommand/Arguments" ) ).toString() );
	cmd.setWorkingDirectory( settings->value( settingsKey( "CompileCommand/WorkingDirectory" ) ).toString() );
	cmd.setParsers( settings->value( settingsKey( "CompileCommand/Parsers" ) ).toStringList() );
	cmd.setTryAllParsers( settings->value( settingsKey( "CompileCommand/TryAll" ), false ).toBool() );
	cmd.setSkipOnError( settings->value( settingsKey( "CompileCommand/SkipOnError" ), false ).toBool() );
	
	// if no user commands get global ones
	if ( !cmd.isValid() )
	{
		cmd = defaultCompileCommand();
	}
	
	return cmd;
}

void CompilerPlugin::setCompileCommand( const pCommand& cmd )
{
	pSettings* settings = MonkeyCore::settings();
	
	settings->setValue( settingsKey( "CompileCommand/Text" ), cmd.text() );
	settings->setValue( settingsKey( "CompileCommand/Command" ), cmd.command() );
	settings->setValue( settingsKey( "CompileCommand/Arguments" ), cmd.arguments() );
	settings->setValue( settingsKey( "CompileCommand/WorkingDirectory" ), cmd.workingDirectory() );
	settings->setValue( settingsKey( "CompileCommand/Parsers" ), cmd.parsers() );
	settings->setValue( settingsKey( "CompileCommand/TryAll" ), cmd.tryAllParsers() );
	settings->setValue( settingsKey( "CompileCommand/SkipOnError" ), cmd.skipOnError() );
}

QWidget* CompilerPlugin::compilerSettingsWidget()
{
	return new UICompilerSettings( this, QApplication::activeWindow() );
}

