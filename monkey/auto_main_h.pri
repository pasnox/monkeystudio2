!build_pass {
    MAIN_H.content = $$cat( main.h.template )

    variables = $$find( MAIN_H.content, \\$\\$\\{[^\\ }]+\\} )

    for( variable, variables ) {
        name = $${variable}
        name = $$replace( name, \", )
        name = $$replace( name, \\$, )
        name = $$replace( name, \\{, )
        name = $$replace( name, \\}, )
        name = $$replace( name, \\\\n, )
        
        MAIN_H.content = $$replace( MAIN_H.content, \\$\\$\\{$${name}\\}, $$eval( $${name} ) )
        
        #message( --- Found: $$variable ($$name) - $$eval( $$name ) )
    }

    MAIN_H.content = $$replace( MAIN_H.content, \", \\\" )
    #MAIN_H.content = $$join( MAIN_H.content )
    #MAIN_H.content = $$replace( MAIN_H.content, "\\\\n ", "\\n" )

    MAIN_H.file = src/main.h

    exists( $${MAIN_H.file} ) {
        win32:!cb_win32:system( del $${MAIN_H.file} )
        else:system( rm $${MAIN_H.file} )
    }

    MAIN_H.target = $$basename( MAIN_H.file )
    win32:!cb_win32:MAIN_H.commands = echo \"$${MAIN_H.content}\" > $${MAIN_H.file}
    else:MAIN_H.commands = echo \"$${MAIN_H.content}\" > $${MAIN_H.file}
    
    message( Generating $${MAIN_H.target}... )
    system( $${MAIN_H.commands} )
}