import re
import copy
import sys

class Pattern:
    """Regular expression pattern. Contains output format info and unit tests for self
        Allowed types are 'error', 'warning', 'compiling'
    """
    def __init__(self, pattern, file = '', line = '-1', column = '-1', type = 'error', text = '%0', hint = '%0'):
        self.pattern = pattern
        self.file = file
        self.line = line
        self.column = column
        self.type = type
        self.text = text
        self.hint = hint
    
    def setComment(self, comment):
        self.comment = comment
    
    def processTemplate(self, template, inputString, match, vars):
        """Replace %x with it's value
        Used for unit tests
        """
        string = copy.copy(template)
        string = string.replace('%0', match) # input string without \n
        for i, val in enumerate(vars):
            if val is None: # (blablabla)? pattern returns None, if not found
                val = ''
            string = string.replace('%%%d' % (i + 1), val)
        return string
    
    
    def parse(self, string):
        """Parse the string and return tuple (file, line, column, type, text, hint)
        Used for unit tests
        """
        res = re.search(self.pattern, string)
        
        if res is None:
            print >> sys.stderr, 'Failed pattern "%s", text <%s>\n' %(self.comment, string)
            assert(None)
        
        match = string[res.start():res.end()]
        d = {}
        d['file'] = self.processTemplate(self.file, string, match, res.groups())
        d['line'] = self.processTemplate(self.line, string, match, res.groups())
        d['column'] = self.processTemplate(self.column, string, match, res.groups())
        d['type'] = self.type
        d['text'] = self.processTemplate(self.text, string, match, res.groups())
        d['hint'] = self.processTemplate(self.hint, string, match, res.groups())
        if '--debug' in sys.argv:
            print 'For <%s>' % self.comment
            print 'Full match: <', res.string[res.start():res.end()], '>'
            print d
        
        return d
    
    def test(self, string, file = '', line = '-1', column = '-1', type = 'error', text = '', hint = ''):
        """Do unit test
        """
        res = self.parse(string)
        
        failed = False
        
        if res['file'] != file:
            print >> sys.stderr, 'file <%s> != <%s>' % (res['file'], file)
            failed = True
        if res['line'] != line:
            print >> sys.stderr, 'line <%s> != <%s>' % (res['line'], line)
            failed = True
        if res['column'] != column:
            print >> sys.stderr, 'column <%s> != <%s>' % (res['column'], column)
            failed = True
        if res['type'] != type:
            print >> sys.stderr, 'type <%s> != <%s>' % (res['type'], type)
            failed = True
        if res['text'] != text:
            print >> sys.stderr, 'text <%s> != <%s>' % (res['text'], text)
            failed = True
        if res['hint'] != hint:
            print >> sys.stderr, 'hint <%s> != <%s>' % (res['hint'], hint)
            failed = True
        assert(not failed)
    
    def generateMkSScript(self, parserName):
        template =  """# %s\nparser add "%s" "%s" "%s" "%s" "%s" "%s" "%s" "%s"
        """         
        arguments = (self.comment, parserName, self.pattern, self.file, self.column, self.line, self.type, self.text, self.hint)
        #arguments = [arg.replace('\\', '\\\\') for arg in arguments]
        arguments = [arg.replace('"', '\\"') for arg in arguments]
        result =  template % tuple(arguments)
        return result
