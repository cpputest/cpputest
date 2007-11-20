#/bin/bash
CPP_U_TEST=$(pwd)/..

EXE_DIR=${EXE_DIR:-/usr/local/bin}
test -f ${EXE_DIR} || mkdir -p ${EXE_DIR}

for file in NewClass NewInterface NewCModule NewProject NewLibrary ; do
   rm -f ${EXE_DIR}/${file}
   ln -s ${CPP_U_TEST}/scripts/${file}.sh ${EXE_DIR}/${file}
   chmod a+x ${EXE_DIR}/${file}
done
