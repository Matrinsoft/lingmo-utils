set(PUBLIC_HEADERS
    ${CMAKE_SOURCE_DIR}/include/LingmoUtils/Logger.h
    ${CMAKE_SOURCE_DIR}/include/LingmoUtils/FileUtils.h
    ${CMAKE_SOURCE_DIR}/include/LingmoUtils/XdgUtils.h
    ${CMAKE_SOURCE_DIR}/include/LingmoUtils/Process.h
)

set(PRIVATE_HEADERS
    LingmoUtils/private/Logger_p.h
    LingmoUtils/private/FileUtils_p.h
    LingmoUtils/private/Process_p.h
)

set(SOURCES
    LingmoUtils/Logger.cpp
    LingmoUtils/FileUtils.cpp
    LingmoUtils/XdgUtils.cpp
    LingmoUtils/Process.cpp
)

set(CMAKE_AUTOMOC ON)

add_library(LingmoUtils SHARED ${SOURCES} ${PUBLIC_HEADERS} ${PRIVATE_HEADERS})
add_library(Lingmo::Utils ALIAS LingmoUtils)

target_include_directories(LingmoUtils
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>
        $<BUILD_INTERFACE:${CMAKE_BINARY_DIR}/include>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(LingmoUtils
    PUBLIC
        Qt6::Core
)

target_compile_definitions(LingmoUtils
    PRIVATE
        LINGMOUTILS_LIBRARY
)

set_target_properties(LingmoUtils PROPERTIES
    SOVERSION 1
    VERSION ${PROJECT_VERSION}
    DEFINE_SYMBOL LINGMOUTILS_LIBRARY
    OUTPUT_NAME LingmoUtils
)

configure_file(
    "${CMAKE_SOURCE_DIR}/include/LingmoUtils/LingmoUtilsExport.h.in"
    "${CMAKE_BINARY_DIR}/include/LingmoUtils/LingmoUtilsExport.h"
)

configure_file(
    "${CMAKE_SOURCE_DIR}/include/LingmoUtils/LingmoUtilsVersion.h.in"
    "${CMAKE_BINARY_DIR}/include/LingmoUtils/LingmoUtilsVersion.h"
)

install(TARGETS LingmoUtils
    EXPORT LingmoUtilsTargets
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

install(DIRECTORY ${CMAKE_SOURCE_DIR}/include/LingmoUtils/
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/LingmoUtils
    FILES_MATCHING PATTERN "*.h"
    PATTERN "*.h.in" EXCLUDE
)

install(FILES
    "${CMAKE_BINARY_DIR}/include/LingmoUtils/LingmoUtilsExport.h"
    "${CMAKE_BINARY_DIR}/include/LingmoUtils/LingmoUtilsVersion.h"
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/LingmoUtils
)

install(EXPORT LingmoUtilsTargets
    FILE LingmoUtilsTargets.cmake
    NAMESPACE Lingmo::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/LingmoUtils
)

include(CMakePackageConfigHelpers)
configure_package_config_file(
    "${CMAKE_SOURCE_DIR}/cmake/LingmoUtilsConfig.cmake.in"
    "${CMAKE_BINARY_DIR}/LingmoUtilsConfig.cmake"
    INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/LingmoUtils
    PATH_VARS CMAKE_INSTALL_INCLUDEDIR CMAKE_INSTALL_LIBDIR
)

write_basic_package_version_file(
    "${CMAKE_BINARY_DIR}/LingmoUtilsConfigVersion.cmake"
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY SameMajorVersion
)

install(FILES
    "${CMAKE_BINARY_DIR}/LingmoUtilsConfig.cmake"
    "${CMAKE_BINARY_DIR}/LingmoUtilsConfigVersion.cmake"
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/LingmoUtils
)
