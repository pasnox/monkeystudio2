mac {
    greaterThan(QT_MAJOR_VERSION, 4) {
        greaterThan(QT_MINOR_VERSION, 0) {
            QMAKE_MAC_SDK = macosx
        } else {
            QMAKE_MAC_SDK = $$system("xcrun --show-sdk-path")
        }
    }
}
