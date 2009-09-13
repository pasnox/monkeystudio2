import re
import copy

class RegEx:
	"""Regular expression pattern. Contains output format info and tests for self
		Allowed types are 'error', 'warning', 'compiling'
	"""
	def __init__(self, pattern, case = True, file = '', line = '-1', column = '-1', type = 'error', text = '%0', hint = '%0'):
		self.pattern = pattern
		self.case = case
		self.file = file
		self.line = line
		self.column = column
		self.type = type
		self.text = text
		self.hint = hint
	
	def setComment(self, comment):
		self.comment = comment
	
	def processTemplate(self, template, inputString, vars):
		"""Replace %x with it's value
		"""
		string = copy.copy(template)
		string = string.replace('%0', inputString)
		for i, val in enumerate(vars):
			string = string.replace('%%%d' % (i + 1), val[0])
		
		return string
	
	
	def parse(self, string):
		"""Parse the string and return tuple (file, line, column, type, text, hint)
		"""
		res = re.findall(self.pattern, string, re.I and not self.case)
		file = self.processTemplate(self.file, string, res)
		line = self.processTemplate(self.line, string, res)
		column = self.processTemplate(self.column, string, res)
		type = self.type
		text = self.processTemplate(self.text, string, res)
		hint = self.processTemplate(self.hint, string, res)
		
		return (file, line, column, type, text, hint)
	
	def test(self, string, file = '', line = '-1', column = '-1', type = 'error', text = 'text here', hint = 'hint here'):
		res = self.parse(string)
		
		failed = False
		
		if res[0] != file:
			print 'file <%s> != <%s>' % (res[0], file)
			failed = True
		if res[1] != line:
			print 'line <%s> != <%s>' % (res[1], line)
			failed = True
		if res[2] != column:
			print 'column <%s> != <%s>' % (res[2], column)
			failed = True
		if res[3] != type:
			print 'type <%s> != <%s>' % (res[3], type)
			failed = True
		if res[4] != text:
			print 'text <%s> != <%s>' % (res[4], text)		
			failed = True
		if res[5] != hint:
			print 'hint <%s> != <%s>' % (res[5], hint)
			failed = True
		return failed
	
	def generateCppCode(self):
		template = 	"""
		{   // %s
			QRegExp("%s",
							%s, 
							QRegExp::RegExp2), //reg exp
			"%s", //file name
			"%s", //column
			"%s", //row
			%s, //type
			"%s", //text
			"%s" //full text
		},
		""" 
		if self.case:
			case = 'Qt::CaseSensitive'
		else:
			case = 'Qt::CaseInsensitive'
		
		if self.type == 'error':
			type = 'pConsoleManager::stError'
		elif self.type == 'warning':
			type = 'pConsoleManager::stWarning'
		elif self.type == 'compiling':
			type = 'pConsoleManager::stCompiling'
		
		result =  template % (self.comment, self.pattern, case, self.file, self.column, self.line, type, self.text, self.hint)
		return result.replace('\\', '\\\\')















