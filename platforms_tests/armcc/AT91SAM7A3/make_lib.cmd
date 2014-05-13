make -C ../../../platforms/armcc clean
make -C ../../../platforms/armcc all CPPUTEST_USE_STD_CPP_LIB=Y CPPUTEST_USE_MEM_LEAK_DETECTION=N
