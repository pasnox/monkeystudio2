from PyQt4 import uic

( Ui_$Class Name$, $Qt Widget Class$ ) = uic.loadUiType( '$Class Name$.ui' )

class $Class Name$ ( $Qt Widget Class$ ):
    """$Class Name$ inherits $Qt Widget Class$"""

    def __init__ ( self, parent = None ):
        $Qt Widget Class$.__init__( self, parent )
        self.ui = Ui_$Class Name$()
        self.ui.setupUi( self )

    def __del__ ( self ):
        self.ui = None
