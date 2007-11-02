# fresh includes path to include in plugins/core project 

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