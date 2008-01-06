# monkey includes path to include in plugins/core project 

# get relative path
QPRO_PWD   = $$PWD

# include path
# current
INCLUDEPATH	*= $${QPRO_PWD}

# default
INCLUDEPATH	*= $${QPRO_PWD}/. $${QPRO_PWD}/src/. . src

# Core
INCLUDEPATH	*= $${QPRO_PWD}/src/coremanager

# Main Interface
INCLUDEPATH	*= $${QPRO_PWD}/src/maininterface $${QPRO_PWD}/src/maininterface/ui

# Workspace Manager
INCLUDEPATH	*= $${QPRO_PWD}/src/workspace

# Recents Manager
INCLUDEPATH	*= $${QPRO_PWD}/src/recentsmanager

# QScintilla Manager
INCLUDEPATH	*= $${QPRO_PWD}/src/qscintillamanager $${QPRO_PWD}/src/qscintillamanager/ui

# Ctags Manager
INCLUDEPATH	*= $${QPRO_PWD}/src/ctagsmanager

# Variables Manager
INCLUDEPATH	*= $${QPRO_PWD}/src/variablesmanager

# Templates Manager
INCLUDEPATH	*= $${QPRO_PWD}/src/templatesmanager $${QPRO_PWD}/src/templatesmanager/ui

# Abbreviations Manager
INCLUDEPATH	*= $${QPRO_PWD}/src/abbreviationsmanager $${QPRO_PWD}/src/abbreviationsmanager/ui

# Tools Manager
INCLUDEPATH	*= $${QPRO_PWD}/src/toolsmanager $${QPRO_PWD}/src/toolsmanager/ui

# Console Manager
INCLUDEPATH	*= $${QPRO_PWD}/src/consolemanager $${QPRO_PWD}/src/consolemanager/ui

# Plugins Manager
INCLUDEPATH	*= $${QPRO_PWD}/src/pluginsmanager $${QPRO_PWD}/src/pluginsmanager/ui

# Projects Manager
INCLUDEPATH	*= $${QPRO_PWD}/src/projectsmanager $${QPRO_PWD}/src/projectsmanager/ui
