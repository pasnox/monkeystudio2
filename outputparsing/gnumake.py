import parsing

# generic error rule for make + message
genericErrorMake = parsing.Pattern( r"(?:^|\n)(?:mingw32-)?make(?:\[\d+\])?\s*:(?:\s*\*+)\s*([^\n$]+)",
                                    type = 'error',
                                    text = '%1' )
genericErrorMake.setComment( 'Generic error make message' )

genericErrorMake.test( "mingw32-make: *** No rule to make target `release'.  Stop.\n", 
                type = 'error', 
                text = "No rule to make target `release'.  Stop.",
                hint = "mingw32-make: *** No rule to make target `release'.  Stop." )

# generic warning rule for make + message
genericWarningMake = parsing.Pattern( r"(?:^|\n)(?:mingw32-)?make(?:\[\d+\])?\s*:\s*([^\n$]+)",
                                    type = 'message',
                                    text = '%1' )
genericWarningMake.setComment( 'Generic warning make message' )

genericWarningMake.test( "make: Entering directory `/home/pasnox/Temporaire/cppqtgui'\n",
                type = 'message',
                text = "Entering directory `/home/pasnox/Temporaire/cppqtgui'",
                hint = "make: Entering directory `/home/pasnox/Temporaire/cppqtgui'" )

genericWarningMake.test( "make: Leaving directory `/home/pasnox/Temporaire/cppqtgui'\n",
                type = 'message',
                text = "Leaving directory `/home/pasnox/Temporaire/cppqtgui'",
                hint = "make: Leaving directory `/home/pasnox/Temporaire/cppqtgui'" )

genericWarningMake.test( "make: Nothing to be done for `first'.\n",
                type = 'message',
                text = "Nothing to be done for `first'.",
                hint = "make: Nothing to be done for `first'." )

# Generation of script file
print '# It is a machine generated file. Do not edit it manualy!'
print ''
print genericErrorMake.generateMkSScript( 'GNU Make' )
print genericWarningMake.generateMkSScript( 'GNU Make' )