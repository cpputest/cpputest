
- install Posix libraries (S60 Open C or Symbian PIPS), I used Open C
- install STL, I used this one: 
http://marcoplusplus.blogspot.com/2007/05/stlport-for-symbian-os-released.html
- now compilation should succeed in regular Symbian way. (in 
build-directory issue commands bldmake bldfiles and abld build)
- in your tests' project file use STATICLIBRARY cpputest.lib