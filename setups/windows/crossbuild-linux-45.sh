echo "This script MUST be started from root of project (where is located build.pro)"
echo "You can give as first parameter the Qt version to use, eg: 4.5.1"

QT_WIN32_VERSION="4.5.1"

if [ '!' -z "$1" ]; then
	QT_WIN32_VERSION=$1
fi

export QT_WIN32_VERSION

echo "Using Qt Version: $QT_WIN32_VERSION"

qmake-qt4 -spec $HOME/.qt/win32-x11-g++ -win32 -r
make -j4
wine "$HOME/Disk Wine/Program Files/Inno Setup 5/ISCC.exe" setups/windows/monkeystudio.iss