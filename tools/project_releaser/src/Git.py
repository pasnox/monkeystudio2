import Project
import Tools
import Qt
import Svn
import Wine

class Git:
    def __init__(self, url, login = None, password = None):
        self.url = url
        self.login = login
        self.password = password
