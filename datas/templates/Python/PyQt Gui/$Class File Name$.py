from PyQt4 import uic

( Ui_$Class Name$, $Base Class$ ) = uic.loadUiType( '$Class File Name$.ui' )

class $Class Name$ ( $Base Class$ ):
    """$Class Name$ inherits $Base Class$"""

    def __init__ ( self, parent = None ):
        $Base Class$.__init__( self, parent )
        self.ui = Ui_$Class Name$()
        self.ui.setupUi( self )

    def __del__ ( self ):
        self.ui = None
