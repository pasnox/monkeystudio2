defineReplace( getFolders ) {
	parameters	= $$1
	q_folders	= 
	
	for( q_path, parameters ) {
		win32:command	= "dir \"$$q_path\" /s /b /a:d-h | find /V /I \".svn\" | sed \"s/\\\(.*\\\)/\\\"\1\\\"/\""
		else:command	= "ls -RQ1 \"$$q_path\" | grep \":\" | sed \"s/://g\" | sed \"s/'/\\\\\\'/g\""
		_q_folders	= $$system( $$command )
		
		#message( "Scanning: $$q_path" )
		#message( "Command: $$command" )
		#message( "Folders: $$_q_folders" )
		
		for( q_folder, _q_folders ) {
			exists( $$q_folder ) {
				#message( "Adding: $$q_folder" )
				q_folders	*= $$q_folder
			}
		}
	}
	
	return( $$q_folders )
}