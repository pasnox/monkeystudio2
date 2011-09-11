#!/bin/sh

python gnumake.py > ../datas/scripts/parser-gnumake.mks
python gcc.py > ../datas/scripts/parser-gcc.mks
python qmake.py > ../datas/scripts/parser-qmake.mks
