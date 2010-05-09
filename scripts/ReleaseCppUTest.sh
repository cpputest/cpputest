#/bin/sh -x
#source in release generator script

mkdir -p Releases
GENERATED_SCRIPTS=""
exitIfFileExists() {
  if [ -e $1 ]
    then 
      echo "${1} already exists, exiting."
      exit
  fi
}

generateMakeScript() {
    filename=$1
    target=$2
    dateTime=$3
    version=$4
    exitIfFileExists $filename.sh
    echo "#Generated file - ${filename}.sh"  >$filename.sh
    echo "echo \"Running ${filename} for CppUTest ${version} created on ${dateTime}\"" >>$filename.sh
    echo "export CPPUTEST_HOME=" >>$filename.sh
    echo "export CPPUTEST_ENABLE_EXTENSIONS=Y" >>$filename.sh
    echo "make $target" >>$filename.sh
    echo "make -C examples $target" >>$filename.sh
    chmod +x $filename.sh
    GENERATED_SCRIPTS+=" $filename.sh"
}

zipIt() {
    zip -r Releases/CppUTest-${1}.zip \
           $GENERATED_SCRIPTS \
           .\
           -x@${SCRIPTS_DIR}/zipExclude.txt
}

cleanUp() {
    rm -f $GENERATED_SCRIPTS
}

generateCppUTestRelease() {
    version=$1
    dateTime=$(date +%F-%H-%M)
    generateMakeScript makeAll all $dateTime $version
    generateMakeScript cleanAll clean $dateTime  $version
    zipIt $version
    cleanUp
}


//Main stuff
SCRIPTS_DIR=scripts
version=v2.2
generateCppUTestRelease $version

