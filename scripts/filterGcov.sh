#!/bin/bash
INPUT_FILE=$1
TEMP_FILE1=${INPUT_FILE}1.tmp
TEMP_FILE2=${INPUT_FILE}2.tmp
TEMP_FILE3=${INPUT_FILE}3.tmp
ERROR_FILE=$2
OUTPUT_FILE=$3

flattenGcovOutput() {
while read line1
do
  read line2
  echo $line2 " " $line1 
  read junk
  read junk
done < ${INPUT_FILE}
}

getRidOfCruft() {
sed '-e s/^Lines.*://g' \
    '-e s/^[0-9]\./  &/g' \
    '-e s/^[0-9][0-9]\./ &/g' \
    '-e s/of.*File/ /g' \
    "-e s/'//g" 
}

getFileNameRootFromErrorFile() {
sed '-e s/gc..:cannot open .* file//g' ${ERROR_FILE}
}

writeEachNoTestCoverageFile() {
while read line
do
  echo "  0.00%  " ${line}
done 
}

flattenGcovOutput | getRidOfCruft  > ${TEMP_FILE1}
getFileNameRootFromErrorFile | writeEachNoTestCoverageFile > ${TEMP_FILE2}
cat ${TEMP_FILE1}  ${TEMP_FILE2} | sort | uniq > ${OUTPUT_FILE}
#rm -f ${TEMP_FILE1} ${TEMP_FILE2} 
