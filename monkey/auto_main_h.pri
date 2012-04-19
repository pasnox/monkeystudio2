!build_pass {
    MAIN_H.source = main.h.template
    MAIN_H.target = src/main.h
    MAIN_H.content = $$cat( $${MAIN_H.source}, false )

    variables = $$find( MAIN_H.content, "\\$\\$\\{[^}]+\\}" )

    for( variable, variables ) {
        name = $${variable}
        name = $$replace( name, $${Q_QUOTE}, "" )
        name = $$replace( name, $${Q_DOLLAR}, "" )
        name = $$replace( name, $${Q_OPENING_BRACE}, "" )
        name = $$replace( name, $${Q_CLOSING_BRACE}, "" )
        
        MAIN_H.content = $$replace( MAIN_H.content, $${Q_DOLLAR}$${Q_DOLLAR}$${Q_OPENING_BRACE}$${name}$${Q_CLOSING_BRACE}, $$eval( $${name} ) )
        
        #message( --- Found: $$variable ($$name) - $$eval( $$name ) )
    }

    MAIN_H.content = $$replace( MAIN_H.content, $${Q_BACK_SLASH}$${Q_BACK_SLASH}, $${Q_BACK_SLASH}$${Q_BACK_SLASH}$${Q_BACK_SLASH} )
    MAIN_H.content = $$replace( MAIN_H.content, $${Q_QUOTE}, $${Q_BACK_SLASH}$${Q_QUOTE} )

    exists( $${MAIN_H.target} ) {
        win32:!cb_win32:system( del $${MAIN_H.target} )
        else:system( rm $${MAIN_H.target} )
    }

    win32:!cb_win32:MAIN_H.commands = echo \" $${MAIN_H.content}\" > $${MAIN_H.target}
    else:MAIN_H.commands = echo \" $${MAIN_H.content}\" > $${MAIN_H.target}
    
    message( Generating $${MAIN_H.target}... )
    system( $${MAIN_H.commands} )
}