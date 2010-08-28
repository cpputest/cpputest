#/bin/sh -x
#source in release generator script

GENERATED_FILES=""
release_dir=Releases
scripts_dir=scripts
version=v2.2d
zip_root=CppUTest-${version}
zip_file=${zip_root}.zip

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
    echo "export CPPUTEST_HOME=" >>$filename.sh
    echo "export CPPUTEST_ENABLE_EXTENSIONS=Y" >>$filename.sh
    echo "make $target" >>$filename.sh

    if [ ${target} == "clean" ] 
        then
            echo "make cleanExamples" >>$filename.sh
        else
            echo "make examples" >>$filename.sh
    fi
                    
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
}

cleanUp() {
    rm -f $GENERATED_FILES
}

generateCppUTestRelease() {
    dateTime=$(date +%F-%H-%M)
    generateVersionFile $version $dateTime
    generateMakeScript makeAll $dateTime $version all
    generateMakeScript cleanAll $dateTime  $version clean
    zipIt $version
    cleanUp
}

openAndMakeRelease()
{
    cd ${release_dir}
    rm -rf ${zip_root}
    unzip ${zip_file} -d ${zip_root}
    cd ${zip_root}
    ./makeAll.sh
    cd ..
}


#Main
generateCppUTestRelease
openAndMakeRelease
