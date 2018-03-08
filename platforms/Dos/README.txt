To compile on your Windows PC (or other platform), please create
a file called "platform.mk" in this directory.

### Example platform.mk for use with Cygwin on Windows XP
WATCOM := d:\\bin\\watcom
CPPUTEST_HOME := /d/dev/05_CppUTest/cpputest
DOSBOX := DOSBox
CYGDRIVE:= /d/
path_separator := \\

### Another example platform.mk for use with Cygwin on Windows 7
WATCOM := d:\watcom
CPPUTEST_HOME := /cygdrive/c/data/00_Dev/05_CppUTest/cpputest
INCLUDE := c:\data\00_Dev\05_CppUTest\cpputest\include
DOSBOX := DOSBox
CYGDRIVE:= /cygdrive/d/
path_separator := \\

### platform.mk for use with Cygwin 2.5.0 on Windows 7
WATCOM := d:\watcom
CPPUTEST_HOME := c:\\data\\00_Dev\\05_CppUTest\\cpputest
INCLUDE := c:\data\00_Dev\05_CppUTest\cpputest\include
DOSBOX := DOSBox
CYGDRIVE:= /cygdrive/d/
path_separator := \\
