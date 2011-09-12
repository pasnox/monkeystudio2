import parsing

# No rule for make target

noRule = parsing.Pattern(r"^((mingw32\-)?make: \*\*\* No rule to make target.*)  Stop.",
                                type = 'error',
                                text = '%1')
noRule.setComment('No rule for make target')

noRule.test("mingw32-make: *** No rule to make target `release'.  Stop.\n", 
                type = 'error', 
                text = "mingw32-make: *** No rule to make target `release'.",
                hint = "mingw32-make: *** No rule to make target `release'.  Stop.")

# Entering directory

entering = parsing.Pattern(r"^(mingw32\-)?make\[\d\]: Entering directory\s`([^\n]*)'",
                                        type = 'compiling',
                                        text = 'make: Building %2')
entering.setComment('Entering dirrectory')
#TODO create test

print '# It is a machine generated file. Do not edit it manualy!'

print noRule.generateMkSScript('GNU Make')
print entering.generateMkSScript('GNU Make')
