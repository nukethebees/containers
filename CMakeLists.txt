cmake_minimum_required (VERSION 3.28.3)

project (containers
    LANGUAGES CXX
    VERSION 0.3.0
)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# ASM output
# ##############################################
make_directory(${CMAKE_BINARY_DIR}/asm)

# Sub-folders
# ##############################################
add_subdirectory ("containers")
add_subdirectory ("tests")
add_subdirectory ("benchmarks")

# Packaging
# ##############################################
include(GNUInstallDirs)
include(CMakePackageConfigHelpers)

configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/cmake/containers.pc.in
    ${CMAKE_CURRENT_BINARY_DIR}/containers.pc
)

install(
    FILES ${CMAKE_CURRENT_BINARY_DIR}/containers.pc
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/pkgconfig
)

write_basic_package_version_file(
    ${CMAKE_CURRENT_BINARY_DIR}/containers-config-version.cmake
    COMPATIBILITY AnyNewerVersion
)

install(FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/cmake/containers-config.cmake
    ${CMAKE_CURRENT_BINARY_DIR}/containers-config-version.cmake
    DESTINATION ${CMAKE_INSTALL_DATADIR}/containers
)

install(
    TARGETS containers
    EXPORT containersTargets
    FILE_SET HEADERS
)

install(
    EXPORT containersTargets
    NAMESPACE containers::
    DESTINATION ${CMAKE_INSTALL_DATADIR}/containers
)