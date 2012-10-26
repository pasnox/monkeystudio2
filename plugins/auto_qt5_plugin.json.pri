# Mimic an auto generated file where the content is replaced with qmake variable values
# $$1 = Template source file path
# $$2 = Generated target file path
defineTest( autoGenerateFile ) {
    Q_HOST_OS = $${QMAKE_HOST.os}
    Q_LOWER_HOST_OS = $$lower( $${Q_HOST_OS} )

    win32 {
        !isEqual(  Q_LOWER_HOST_OS, "windows" ) {
            # we are cross building for windows
            CONFIG *= cb_win32
        }
    }

    macx {
        !isEqual(  Q_LOWER_HOST_OS, "darwin" ) {
            # we are cross building for mac os x
            CONFIG *= cb_mac
        }
    }

    # the build target os
    Q_TARGET = $${Q_HOST_OS}
    # the build target arch
    Q_ARCH = $${QT_ARCH}

    cb_win32 {
    Q_TARGET = Windows
    }

    cb_mac {
    Q_TARGET = Darwin
    }

    Q_TARGET_ARCH = "$${Q_TARGET}-$${Q_ARCH}"

    # Some useful variables that can't be easily added in variable values directly
    Q_NULL =
    Q_BACK_SLASH = "\\"
    Q_SLASH = "/"
    Q_QUOTE = "\""
    Q_DOLLAR = "\\$"
    Q_OPENING_BRACE = "\\{"
    Q_CLOSING_BRACE = "\\}"
    
    
    !build_pass:isEmpty( translations_pass ) {
        generator.source = $${1}
        generator.target = $${2}
        
        # Replace slashes by back slashes on native windows host
        win32:!cb_win32 {
            generator.source = $$replace( generator.source, $${Q_SLASH}, $${Q_BACK_SLASH} )
            generator.target = $$replace( generator.target, $${Q_SLASH}, $${Q_BACK_SLASH} )
        }
        
        # Delete existing file
        exists( $${generator.target} ) {
            win32:!cb_win32 {
                system( "del $${generator.target}" )
            } else {
                system( "rm $${generator.target}" )
            }
        }
        
        # Get template content
        generator.content = $$cat( $${generator.source}, false )
        
        # Find template variables name
        #generator.variables = $$find( generator.content, "\\$\\$[^\s\$]+" )
        
        # Generate the find variables command
        generator.commands = "grep -E -i -o '\\$\\$[$${Q_OPENING_BRACE}]?[^ $${Q_QUOTE}$]+[$${Q_OPENING_BRACE}]?' $${generator.source}"
        win32:!cb_win32:generator.commands = "grep command not available."
        
        # Get template variables name
        generator.variables = $$system( $${generator.commands} )
        
        #message( cmd: $${generator.commands} )
        #message( Variables: $$generator.variables )

        # Transform each variable
        for( variable, generator.variables ) {
            name = $${variable}
            name = $$replace( name, $${Q_QUOTE}, "" )
            name = $$replace( name, $${Q_DOLLAR}, "" )
            name = $$replace( name, $${Q_OPENING_BRACE}, "" )
            name = $$replace( name, $${Q_CLOSING_BRACE}, "" )
            
            generator.content = $$replace( generator.content, $${Q_DOLLAR}$${Q_DOLLAR}$${Q_OPENING_BRACE}$${name}$${Q_CLOSING_BRACE}, $$eval( $${name} ) )
            generator.content = $$replace( generator.content, $${Q_DOLLAR}$${Q_DOLLAR}$${name}, $$eval( $${name} ) )
            #message( --- Found: $$variable ($$name) - $$eval( $$name ) )
        }
        
        # escape characters that are special for windows echo command
        win32:!cb_win32 {
            generator.content = $$replace( generator.content, "\\^", "^^" )
            generator.content = $$replace( generator.content, "<", "^<" )
            generator.content = $$replace( generator.content, ">", "^>" )
            generator.content = $$replace( generator.content, "\\|", "^|" )
            generator.content = $$replace( generator.content, "&", "^&" )
            # these should be escaped too but qmake values can't be ( or ) so we can't replace...
            #generator.content = $$replace( generator.content, "\\(", "^(" )
            #generator.content = $$replace( generator.content, "\\)", "^)" )
        } else {
            #mac:generator.content = $$replace( generator.content, $${Q_BACK_SLASH}, $${Q_BACK_SLASH}$${Q_BACK_SLASH}$${Q_BACK_SLASH} )
            #else:generator.content = $$replace( generator.content, $${Q_BACK_SLASH}$${Q_BACK_SLASH}, $${Q_BACK_SLASH}$${Q_BACK_SLASH}$${Q_BACK_SLASH} )
            generator.content = $$replace( generator.content, $${Q_BACK_SLASH}$${Q_BACK_SLASH}, $${Q_BACK_SLASH}$${Q_BACK_SLASH}$${Q_BACK_SLASH} )
            generator.content = $$replace( generator.content, $${Q_QUOTE}, $${Q_BACK_SLASH}$${Q_QUOTE} )
        }
        
        message( Generating $${generator.target}... )
        
        win32:!cb_win32 {
            generator.content = $$replace( generator.content, "\\n", ">> $${generator.target} && echo." )
            generator.commands = "echo ^ $${generator.content} >> $${generator.target}"
        } else {
            generator.commands = "echo \" $${generator.content}\" > $${generator.target}"
        }
        
        system( $${generator.commands} )
    }
}