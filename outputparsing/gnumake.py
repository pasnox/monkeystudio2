import parsing

noRule = parsing.Pattern(r"^((mingw32\-)?make: \*\*\* No rule to make target.*)  Stop.",
								type = 'error',
								text = '%1')
noRule.setComment('No rule for make target')

noRule.test("mingw32-make: *** No rule to make target `release'.  Stop.", 
				type = 'error', 
				text = "mingw32-make: *** No rule to make target `release'.",
				hint = "mingw32-make: *** No rule to make target `release'.  Stop.")

entering = parsing.Pattern(r"^(mingw32\-)?make\[\d\]: Entering directory\s`([^\n]*)'",
										type = 'compiling',
										text = 'make: Building %2')
entering.setComment('Entering dirrectory')
#TODO create test

# Generation of script file
print '# It is a machine generated file. Do not edit it manualy!'
print ''
print noRule.generateMkSScript( 'GNU Make' )
print entering.generateMkSScript( 'GNU Make' )
