# Create CTest entries for EXECUTABLE in CTestTestfile.cmake
# Overwrite CTestTestfile.cmake with update version.

# Usage:
#
# This script is to be called from ../Modules/CppUTestBuildTimeDiscoverTests.cmake
#
# Steps to generate ADD_TEST() commands build time
# - Read CTestTestfile.cmake
# - Create update entries
# - Remove duplicate entries
# - Write new CTestTestfile.cmake

######################################################################
# helpers
######################################################################
function (buildtime_add_test)
  # Create ADD_TEST() command string
  # - Extract and remove testname from ARGV
  # - Add inner quotes to test arguments
  # - Add "ADD_TEST()", and first and last quote
  # Append result to CTESTTESTS
  list(GET ARGV 0 testname)
  list(REMOVE_AT ARGV 0)
  string (REPLACE ";" "\" \"" TEST_ARGS "${ARGV}")
  set(test_to_add "ADD_TEST(${testname} \"${TEST_ARGS}\")")
  list(APPEND CTESTTESTS ${test_to_add})
  SET(CTESTTESTS ${CTESTTESTS} PARENT_SCOPE)
endfunction()

function (JOIN VALUES GLUE OUTPUT)
  string (REPLACE ";" "${GLUE}" _TMP_STR "${VALUES}")
  set (${OUTPUT} "${_TMP_STR}" PARENT_SCOPE)
endfunction()

function (buildtime_discover_tests EXECUTABLE_CMD DISCOVER_ARG OUTPUT)
  execute_process(COMMAND ${EXECUTABLE_CMD} ${DISCOVER_ARG}
    OUTPUT_VARIABLE _TMP_OUTPUT
    ERROR_VARIABLE DISCOVER_ERR
    RESULT_VARIABLE DISCOVER_ERR)
  if(NOT ${DISCOVER_ERR} EQUAL 0)
    message(SEND_ERROR "Executable \"${EXECUTABLE_CMD} ${DISCOVER_ARG}\" failed with output:\n"
      "${DISCOVER_ERR}\n"
      "Please check that the excutable was added.")
  endif(NOT ${DISCOVER_ERR} EQUAL 0)
  separate_arguments(_TMP_OUTPUT)
  set(${OUTPUT} "${_TMP_OUTPUT}" PARENT_SCOPE)
endfunction()


######################################################################
# Implementation
######################################################################

set(CTESTFNAME "${CMAKE_CURRENT_BINARY_DIR}/CTestTestfile.cmake")
file(STRINGS ${CTESTFNAME} CTESTTESTS)
set(EXECUTABLE_CMD "${CMAKE_CURRENT_BINARY_DIR}/${EXECUTABLE}")

if (TESTS_DETAILED)
  set(DISCOVER_ARG "-ln")
  buildtime_discover_tests("${EXECUTABLE_CMD}" "${DISCOVER_ARG}" TestList_GroupsAndNames)
  set(lastgroup "")
  foreach(testfullname ${TestList_GroupsAndNames})
    string(REGEX MATCH "^([^/.]+)" groupname ${testfullname})
    string(REGEX MATCH "([^/.]+)$" testname ${testfullname})
    if (NOT ("${groupname}" STREQUAL "${lastgroup}"))
      message("TestGroup: ${groupname}:")
      set(lastgroup "${groupname}")
    endif (NOT ("${groupname}" STREQUAL "${lastgroup}"))
    message("... ${testname}")
    buildtime_add_test(${EXECUTABLE}.${testfullname} ${EXECUTABLE_CMD} -sg ${groupname} -sn ${testname} -c)
  endforeach()
else (TESTS_DETAILED)
  set(DISCOVER_ARG "-lg")
  buildtime_discover_tests("${EXECUTABLE_CMD}" "${DISCOVER_ARG}" TestList_Groups)
  foreach(group ${TestList_Groups})
    message("TestGroup: ${group}")
    buildtime_add_test(${EXECUTABLE}.${group} "${EXECUTABLE_CMD}" -sg ${group} -c)
  endforeach()
endif (TESTS_DETAILED)


# create separate CTest test for each CppUTestTests test

list(REMOVE_DUPLICATES CTESTTESTS)
JOIN("${CTESTTESTS}" "\n" CTESTTESTS)
file(WRITE ${CTESTFNAME} "${CTESTTESTS}\n")
