cmake_minimum_required(VERSION 3.10)
project(openga VERSION 2.0.0 LANGUAGES CXX)

include(CMakePackageConfigHelpers)

find_package(Threads)

# Options
option(Examples "Build the project examples." OFF)

set(OPENGA_TARGET_NAME                ${PROJECT_NAME})
set(OPENGA_CONFIG_INSTALL_DIR         "lib/cmake/${PROJECT_NAME}" CACHE INTERNAL "")
set(OPENGA_INCLUDE_BUILD_DIR          "${PROJECT_SOURCE_DIR}/include/")
set(OPENGA_INCLUDE_INSTALL_DIR        "include")
set(OPENGA_TARGETS_EXPORT_NAME        "${PROJECT_NAME}Targets")
set(OPENGA_CMAKE_CONFIG_TEMPLATE      "cmake/config.cmake.in")
set(OPENGA_CMAKE_CONFIG_DIR           "${CMAKE_CURRENT_BINARY_DIR}")
set(OPENGA_CMAKE_VERSION_CONFIG_FILE  "${OPENGA_CMAKE_CONFIG_DIR}/${PROJECT_NAME}ConfigVersion.cmake")
set(OPENGA_CMAKE_PROJECT_CONFIG_FILE  "${OPENGA_CMAKE_CONFIG_DIR}/${PROJECT_NAME}Config.cmake")
set(OPENGA_CMAKE_PROJECT_TARGETS_FILE "${OPENGA_CMAKE_CONFIG_DIR}/${PROJECT_NAME}Targets.cmake")

add_library(openga INTERFACE)
target_include_directories(openga INTERFACE 
    $<BUILD_INTERFACE:${OPENGA_INCLUDE_BUILD_DIR}>
	$<INSTALL_INTERFACE:include>
)
target_link_libraries(openga INTERFACE Threads::Threads)
target_compile_features(openga INTERFACE
    cxx_std_17
)

write_basic_package_version_file(
    ${OPENGA_CMAKE_VERSION_CONFIG_FILE} COMPATIBILITY SameMajorVersion
)

configure_file(
    ${OPENGA_CMAKE_CONFIG_TEMPLATE}
    ${OPENGA_CMAKE_PROJECT_CONFIG_FILE}
    @ONLY
)

install(
	DIRECTORY ${OPENGA_INCLUDE_BUILD_DIR}
	DESTINATION ${OPENGA_INCLUDE_INSTALL_DIR}
)

install(
	FILES ${OPENGA_CMAKE_PROJECT_CONFIG_FILE} ${OPENGA_CMAKE_VERSION_CONFIG_FILE}
	DESTINATION ${OPENGA_CONFIG_INSTALL_DIR}
)

export(
	TARGETS ${OPENGA_TARGET_NAME}
	NAMESPACE ${PROJECT_NAME}::
	FILE ${OPENGA_CMAKE_PROJECT_TARGETS_FILE}
)

install(
	TARGETS ${OPENGA_TARGET_NAME}
	EXPORT ${OPENGA_TARGETS_EXPORT_NAME}
	INCLUDES DESTINATION ${OPENGA_INCLUDE_INSTALL_DIR}
)

install(
	EXPORT ${OPENGA_TARGETS_EXPORT_NAME}
	NAMESPACE ${PROJECT_NAME}::
	DESTINATION ${OPENGA_CONFIG_INSTALL_DIR}
)

# build configuration for all the examples
if(Examples)
	set(EXAMPLES_DIR "examples")

	add_executable(count-ones "${EXAMPLES_DIR}/count-ones.cpp")
	target_include_directories(count-ones PRIVATE ${OPENGA_INCLUDE_BUILD_DIR})
	target_link_libraries(count-ones PRIVATE openga)
endif()
