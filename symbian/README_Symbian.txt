- Copy SymbianAllTests.cpp to tests/AllTests.cpp
- Copy alltests.mmp to tests
- Install Posix libraries (S60 Open C or Symbian PIPS), I used Open C
- Install STL, I used this one: 
http://marcoplusplus.blogspot.com/2007/05/stlport-for-symbian-os-released.html
- Now compilation should succeed in regular Symbian way. (in 
build-directory issue commands bldmake bldfiles and abld build)
- In your tests' project file use STATICLIBRARY cpputest.lib