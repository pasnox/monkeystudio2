defineReplace( getFolders ) {
	q_paths	= $$1
	q_filters	= $$2 \
		.svn
	q_folders	=

	for( q_path, q_paths ) {
		#win32:command	= "dir \"$$q_path\" /s /b /a:d-h | find /V /I \".svn\" | sed \"s/\\\(.*\\\)/\\\"\1\\\"/\""
		#win32:command	= "for /f %i in ('dir \"$$q_path\" /s /b /a:d-h') do @echo \"%i\" | find /V /I \".svn\""
		win32:command	= "for /D /R \"$$q_path\" %i in (*) do @echo \"%i\""
		else:command	= "ls -RQ1 \"$$q_path\" | grep \":\" | sed \"s/://g\" | sed \"s/'/\\\\\\'/g\""
		_q_folders	= $$system( $$command )

		#message( "Scanning: $$q_path" )
		#message( "Command: $$command" )
		#message( "Folders: $$_q_folders" )
		#message( "Filters: $$q_filters" )

		# loop paths
		for( q_folder, _q_folders ) {
			# check filters
			filtered = false

			for( q_filter, q_filters ) {
				result = $$find( q_folder, $$q_filter )
				!isEmpty( result ):filtered = true
				#message( "Searching '$$q_filter': $$result" )
			}

			#message( "Filtered: $$filtered" )

			isEqual( filtered, false ):exists( $$q_folder ) {
				q_folders	*= $$q_folder
				#message( "Added: $$q_folder" )
			}
		}
	}

	return( $$q_folders )
}
