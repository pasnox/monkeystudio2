XUP.QT_VERSION	= Qt System (Qt-4.7.4)

HEADERS *= test \
	okmichem \
	kool

XUP.Command = mBuilder \
    mac_clean_pack \
    Mac OS X Package \
    /bin/bash -c "cd $cpp$ && ./script.sh" \
    $cpp$ \
     \
    0 \
    1
XUP.Command = mBuilder \
    mac_clean_pack \
    Mac OS X Package \
    /bin/bash -c "cd $cpp$ && ./script.sh" \
    $cpp$ \
     \
    0 \
    1

RC_FILE	*= app_icons/test.rc