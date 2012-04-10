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

!build_pass {
    exists( $${MAIN_H.file} ) {
        win32:!cb_win32:system( del $${MAIN_H.file} )
        else:system( rm $${MAIN_H.file} )
    }
}

MAIN_H.target = $$basename( MAIN_H.file )
win32:!cb_win32:MAIN_H.commands = @echo \" $${MAIN_H.content}\" > $${MAIN_H.file}
else:MAIN_H.commands = @echo \" $${MAIN_H.content}\" > $${MAIN_H.file}
MAIN_H.depends = MAIN_H_ECHO

TARGET.depends = MAIN_H_ECHO

MAIN_H_ECHO.commands = @echo Generating $${MAIN_H.target}
#MAIN_H_ECHO.depends = src/main.h

QMAKE_EXTRA_TARGETS += MAIN_H MAIN_H_ECHO