To compile on your Windows PC (or other platform), please create
a file called "platform.mk" in this directory. The following works
for my particular Cygwin installation and can serve as an example
of what variables you might want to set:

WATCOM := d:\watcom
DOSBOX_HOME := /cygdrive/d/bin/DOSBox
CPPUTEST_HOME := /cygdrive/c/data/00_dev/05_CppUTest/cpputest
DOSBOX :=DOSBox
EXE := .EXE
exe := .exe