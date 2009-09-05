import parsing

noRule = parsing.RegEx(r"^((mingw32\-)?make: \*\*\* No rule to make target.*)  Stop.",
								type = 'error',
								text = 'FUCK %1')
noRule.setComment('No rule for make target')

noRule.test("mingw32-make: *** No rule to make target `release'.  Stop.", 
				type = 'error', 
				text = "mingw32-make: *** No rule to make target `release'.",
				hint = "mingw32-make: *** No rule to make target `release'.  Stop.")

entering = parsing.RegEx(r"^(mingw32\-)?make\[\d\]: Entering directory\s`([^\n]*)'\n",
										type = 'compiling',
										text = 'make: Building %2')
entering.setComment('Entering dirrectory')
#TODO create test

print '//WARNING machine generated code. Do not edit'
print noRule.generateCppCode()
print entering.generateCppCode()