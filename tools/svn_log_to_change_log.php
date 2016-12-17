#!/usr/bin/php
<?php
function usage( $argc, $argv )
{
	echo "SvnLog to ChangeLog.\n";
	echo "\n";
	echo "Usage:	". $argv[ 0 ] ." [path_to_check] [output_file_path]\n";
	echo "\n";
	echo "	path_to_check is the svn working copy path to check for log, defaulting to ../branches/dev.\n";
	echo "	output_file_path is an optionnal file path to write the result to, defaulting to ../branches/dev/ChangeLog\n";
}

function generateChangeLog( $argc, $argv )
{
	$path = "../branches/dev";
	$fileName = "../branches/dev/ChangeLog";
	$result = -1;
	
	if ( $argc > 1 )
	{
		$path = $argv[ 1 ];
	}
	
	if ( $argc > 2 )
	{
		$fileName = $argv[ 2 ];
	}
	
	system( "svn2cl/svn2cl.sh --include-rev --break-before-msg=2 --output=\"$fileName\" \"$path\"", $result );
	
	if ( $result != 0 )
	{
		echo "You can try to download the used tool here: http://arthurdejong.org/svn2cl/svn2cl-0.12.tar.gz\n";
		echo "And extract it here in folder: svn2cl.\n";
	}
}

if ( $argc == 2 && ( $argv[ 1 ] == "-h" || $argv[ 1 ] == "--help" ) )
{
	usage( $argc, $argv );
	exit;
}

generateChangeLog( $argc, $argv );
?>