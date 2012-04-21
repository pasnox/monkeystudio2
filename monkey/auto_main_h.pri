!build_pass {
    MAIN_H.source = main.h.in
    MAIN_H.target = src/main.h
	
	win32:!cb_win32 {
		MAIN_H.source = $$replace( MAIN_H.source, $${Q_SLASH}, $${Q_BACK_SLASH} )
		MAIN_H.target = $$replace( MAIN_H.target, $${Q_SLASH}, $${Q_BACK_SLASH} )
	}
	
	exists( $${MAIN_H.target} ) {
        win32:!cb_win32 {
			system( "del $${MAIN_H.target}" )
		} else {
			system( "rm $${MAIN_H.target}" )
		}
    }
	
	MAIN_H.content = $$cat( $${MAIN_H.source}, false )
    MAIN_H.variables = $$find( MAIN_H.content, "\\$\\$\\{[^}]+\\}" )

    for( variable, MAIN_H.variables ) {
        name = $${variable}
        name = $$replace( name, $${Q_QUOTE}, "" )
        name = $$replace( name, $${Q_DOLLAR}, "" )
        name = $$replace( name, $${Q_OPENING_BRACE}, "" )
        name = $$replace( name, $${Q_CLOSING_BRACE}, "" )
        
        MAIN_H.content = $$replace( MAIN_H.content, $${Q_DOLLAR}$${Q_DOLLAR}$${Q_OPENING_BRACE}$${name}$${Q_CLOSING_BRACE}, $$eval( $${name} ) )
        
        #message( --- Found: $$variable ($$name) - $$eval( $$name ) )
    }
	
	# escape characters that are special for windows echo command
	win32:!cb_win32 {
		MAIN_H.content = $$replace( MAIN_H.content, "\\^", "^^" )
		MAIN_H.content = $$replace( MAIN_H.content, "<", "^<" )
		MAIN_H.content = $$replace( MAIN_H.content, ">", "^>" )
		MAIN_H.content = $$replace( MAIN_H.content, "\\|", "^|" )
		MAIN_H.content = $$replace( MAIN_H.content, "&", "^&" )
		# these should be escaped too but qmake values can't be ( or ) so we can't replace...
		#MAIN_H.content = $$replace( MAIN_H.content, "\\(", "^(" )
		#MAIN_H.content = $$replace( MAIN_H.content, "\\)", "^)" )
	} else {
		#mac:MAIN_H.content = $$replace( MAIN_H.content, $${Q_BACK_SLASH}, $${Q_BACK_SLASH}$${Q_BACK_SLASH}$${Q_BACK_SLASH} )
		#else:MAIN_H.content = $$replace( MAIN_H.content, $${Q_BACK_SLASH}$${Q_BACK_SLASH}, $${Q_BACK_SLASH}$${Q_BACK_SLASH}$${Q_BACK_SLASH} )
		MAIN_H.content = $$replace( MAIN_H.content, $${Q_BACK_SLASH}$${Q_BACK_SLASH}, $${Q_BACK_SLASH}$${Q_BACK_SLASH}$${Q_BACK_SLASH} )
		MAIN_H.content = $$replace( MAIN_H.content, $${Q_QUOTE}, $${Q_BACK_SLASH}$${Q_QUOTE} )
	}
	
	message( Generating $${MAIN_H.target}... )
	
	win32:!cb_win32 {
		MAIN_H.content = $$replace( MAIN_H.content, "\\n", ">> $${MAIN_H.target} && echo." )
		MAIN_H.commands = "echo ^ $${MAIN_H.content} >> $${MAIN_H.target}"
		system( $${MAIN_H.commands} )
	} else {
		MAIN_H.commands = "echo \" $${MAIN_H.content}\" > $${MAIN_H.target}"
		system( $${MAIN_H.commands} )
	}
	
	#message( $${MAIN_H.content} )
	#message( $${MAIN_H.commands} )
}