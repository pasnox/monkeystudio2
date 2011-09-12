<?php
include( 'ui_$Class Name$.php' );

class $Class Name$ extends $Qt Widget Class$
{
    public $signals = array( "" );
    public $slots = array( "" );
    protected $ui;

    public function __construct( QWidget $parent = NULL )
    {
        parent::__construct( $parent );
        $ui = new Ui_$Class Name$();
        $ui->setupUi( $this );
    }
    
    public function __destruct()
    {
        unset( $ui );
        parent::__destruct();
    }
 }
?>
