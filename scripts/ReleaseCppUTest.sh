#!/bin/bash
#source in release generator script

echo "Do not use this anymore, please use \"configure; make dist\""
exit

if [ ! -d "scripts" ]; then
  echo "You have to run this script from the CPPUTEST_HOME directory!";
  exit
fi

GENERATED_FILES=""
release_dir=Releases
scripts_dir=scripts
version=v3.3
zip_root=CppUTest-${version}
tar_root=CppUTest-${version}_tar
clean_unzip_root=${zip_root}_clean_unzip
clean_untar_root=${zip_root}_clean_untar
zip_file=${zip_root}.zip
tar_file=${zip_root}.tar.gz

exitIfFileExists() {
  if [ -e $1 ]
    then 
      echo "${1} already exists, exiting."
      exit
  fi
}

generateMakeScript() {
    filename=$1
    dateTime=$2
    version=$3
    target=$4
    exitIfFileExists $filename.sh
    echo "#Generated file - ${filename}.sh"  >$filename.sh
    echo "echo \"Running ${filename} for CppUTest ${version} created on ${dateTime}\"" >>$filename.sh
    echo "export CPPUTEST_HOME=\$(pwd)" >>$filename.sh
    echo "echo \"export CPPUTEST_HOME=\$(pwd)/\"" >>$filename.sh
    echo "make $target" >>$filename.sh

    chmod +x $filename.sh
    GENERATED_FILES+=" $filename.sh"
}

generateVersionFile() {
    version=$1
    dateTime=$2
    versionFile=version.txt
    echo "CppUTest ${version} created on ${dateTime}" > $versionFile
    GENERATED_FILES+=$versionFile
}

zipIt() {
    mkdir -p ${release_dir}
    zip -r ${release_dir}/${zip_file} \
           $GENERATED_FILES \
           .\
           -x@${scripts_dir}/zipExclude.txt
	tar -cvpzf ${release_dir}/${tar_file} \
           -X ./${scripts_dir}/zipExclude.txt \
           $GENERATED_FILES \
           .
}

cleanUp() {
    rm -f $GENERATED_FILES
}

generateCppUTestRelease() {
    dateTime=$(date +%F-%H-%M)
    generateVersionFile $version $dateTime
    generateMakeScript makeAll $dateTime $version everythingInstall
    generateMakeScript cleanAll $dateTime  $version cleanEverythingInstall
    zipIt $version
    cleanUp
}

openAndMakeRelease()
{
    cd ${release_dir}

	# unzip and untar the code and make sure it is the same
    rm -rf ${clean_unzip_root}
    unzip ${zip_file} -d ${clean_unzip_root}
    rm -rf ${clean_untar_root}
	mkdir ${clean_untar_root}
	tar -xvzpf ${tar_file} -C ${clean_untar_root}

    rm -rf ${zip_root}
    unzip ${zip_file} -d ${zip_root}
    cd ${zip_root}
    ./makeAll.sh
    cd ..

    rm -rf ${tar_root}
	mkdir ${tar_root}
    tar -xvzpf ${tar_file} -C ${tar_root}
    cd ${tar_root}
    ./makeAll.sh
    cd ../..
}


#Main
generateCppUTestRelease
openAndMakeRelease
