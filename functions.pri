_OS	= $$lower( $$QMAKE_HOST.os )

defineReplace( getFolders ) {
	q_paths	= $$1
	q_filters	= $$2 \
		.svn
	q_folders	=

	for( q_path, q_paths ) {
		command	= "ls -RQ1 \"$$q_path\" | grep \":\" | sed \"s/://g\" | sed \"s/'/\\\\\\'/g\""
		win32:isEqual( _OS, windows ):command	= "for /D /R \"$$q_path\" %i in (*) do @echo \"%i\""
		_q_folders	= $$system( $$command )
		win32:_q_folders *= $$1

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
