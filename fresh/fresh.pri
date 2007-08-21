# fresh includes path to include in plugins/core project 

# for normal build, comment for build plugins
DEFINES	*= MONKEY_CORE_BUILD

# get relative path
QPRO_PWD   = $$PWD

# include path
# current
INCLUDEPATH	*= $${QPRO_PWD}

# Objects
INCLUDEPATH	*= $${QPRO_PWD}/objects

# Widgets
INCLUDEPATH	*= $${QPRO_PWD}/widgets

# depend path
# current
DEPENDPATH	*= $${QPRO_PWD}

# Objects
DEPENDPATH	*= $${QPRO_PWD}/objects

# Widgets
DEPENDPATH	*= $${QPRO_PWD}/widgets