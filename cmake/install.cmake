set(INCLUDE_INSTALL_DIR "${CMAKE_INSTALL_INCLUDEDIR}")
set(LIB_INSTALL_DIR "${CMAKE_INSTALL_LIBDIR}")
set(INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/include")

# Pkg-config file
set(CppUTest_PKGCONFIG_FILE ${CMAKE_CURRENT_BINARY_DIR}/cpputest.pc)

# Pkg-config file.
set(prefix "${CMAKE_INSTALL_PREFIX}")
set(exec_prefix "\${prefix}")
set(libdir "\${exec_prefix}/${LIB_INSTALL_DIR}")
set(includedir "\${prefix}/${INCLUDE_INSTALL_DIR}")
set(PACKAGE_VERSION "${PROJECT_VERSION}")

configure_file(cpputest.pc.in "${CppUTest_PKGCONFIG_FILE}" @ONLY)
install(
    FILES "${CppUTest_PKGCONFIG_FILE}"
    DESTINATION ${LIB_INSTALL_DIR}/pkgconfig
)

install(
    FILES "${CMAKE_CURRENT_BINARY_DIR}/generated/CppUTestGeneratedConfig.h"
    DESTINATION "${INCLUDE_INSTALL_DIR}/generated"
)

# Try to include helper module
include(CMakePackageConfigHelpers OPTIONAL
    RESULT_VARIABLE PkgHelpers_AVAILABLE
)

# guard against older versions of cmake which do not provide it
if(NOT PkgHelpers_AVAILABLE)
    message(WARNING
        "If you wish to use find_package(CppUTest) in your own project to find CppUTest library"
        " please update cmake to version which provides CMakePackageConfighelpers module"
        " or write generators for CppUTestConfig.cmake by yourself."
    )
    return()
endif()

set(CPPUTEST_CONFIG_DEST "${LIB_INSTALL_DIR}/CppUTest/cmake")

configure_package_config_file(CppUTestConfig.cmake.install.in
    ${CMAKE_CURRENT_BINARY_DIR}/install/CppUTestConfig.cmake
    INSTALL_DESTINATION "${CPPUTEST_CONFIG_DEST}"
    PATH_VARS INCLUDE_INSTALL_DIR LIB_INSTALL_DIR)
write_basic_package_version_file(
    ${CMAKE_CURRENT_BINARY_DIR}/install/CppUTestConfigVersion.cmake
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY SameMajorVersion )
install(
    FILES
        ${CMAKE_CURRENT_BINARY_DIR}/install/CppUTestConfig.cmake
        ${CMAKE_CURRENT_BINARY_DIR}/install/CppUTestConfigVersion.cmake
    DESTINATION "${CPPUTEST_CONFIG_DEST}" )
install(
    EXPORT CppUTestTargets
    NAMESPACE CppUTest::
    DESTINATION "${CPPUTEST_CONFIG_DEST}"
)
install(
    FILES ${CMAKE_CURRENT_SOURCE_DIR}/cmake/Scripts/CppUTestBuildTimeDiscoverTests.cmake
    DESTINATION "${CPPUTEST_CONFIG_DEST}/Scripts"
)
install(
    FILES
        ${CMAKE_CURRENT_SOURCE_DIR}/cmake/Modules/CppUTestBuildTimeDiscoverTests.cmake
        ${CMAKE_CURRENT_SOURCE_DIR}/cmake/Modules/CppUTest.cmake
        ${CMAKE_CURRENT_SOURCE_DIR}/cmake/Modules/_CppUTestDiscovery.cmake
    DESTINATION "${CPPUTEST_CONFIG_DEST}/Modules"
)
configure_package_config_file(CppUTestConfig.cmake.build.in
    ${CMAKE_CURRENT_BINARY_DIR}/CppUTestConfig.cmake
    INSTALL_DESTINATION ${CMAKE_CURRENT_BINARY_DIR}
    PATH_VARS INCLUDE_DIR CMAKE_CURRENT_BINARY_DIR
)
write_basic_package_version_file(
    ${CMAKE_CURRENT_BINARY_DIR}/CppUTestConfigVersion.cmake
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY SameMajorVersion
)
