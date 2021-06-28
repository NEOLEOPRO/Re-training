cmake_minimum_required(VERSION 3.17)
project(Re_training)
set(CMAKE_CXX_STANDARD 20)
add_library(default INTERFACE)
target_compile_features(default INTERFACE cxx_std_20)

set(SFML_STATIC_LIBRARIES TRUE)
set(SFML_DIR "extlibs/SFML-2.5.1/lib/cmake/SFML")
find_package(SFML 2.5.1 COMPONENTS graphics audio REQUIRED)

include(cmake/CompilerWarnings.cmake)
set_project_warnings(default)
include(cmake/conan.cmake)
get_property(languages GLOBAL PROPERTY ENABLED_LANGUAGES)
set(ENV_FOR_C CC)
set(ENV_FOR_CXX CXX)
set(ENV_FOR_Fortran FC)
foreach(language ${languages})
    if(ENV_FOR_${language})
        list(APPEND ENV "${ENV_FOR_${language}}=${CMAKE_${language}_COMPILER}")
    else()
        list(APPEND ENV "${language}=${CMAKE_${language}_COMPILER}")
    endif()
endforeach()
conan_cmake_run(
        CONANFILE
        conanfile.txt
        BASIC_SETUP
        CMAKE_TARGETS
        BUILD
        missing
        ENV
        CONAN_CMAKE_PROGRAM="${CMAKE_COMMAND}"
        ${ENV})

# Создавать докуменатцию с помощью Doxygen в форматах xml и html xml - для
# использования в Sphinx, html - для конечного пользователя путь к документации:
# build/html
set(DOXYGEN_GENERATE_XML YES)
set(DOXYGEN_GENERATE_TREEVIEW YES)
set(DOXYGEN_SOURCE_BROWSER YES)
set(DOXYGEN_INLINE_SOURCES YES)
set(DOXYGEN_HTML_EXTRA_STYLESHEET doxygen-awesome.css)
find_package(Doxygen)
doxygen_add_docs(Doxygen src)

# Создавать документацию с помощью sphinx путь к документации: build/docs
add_custom_target(
        Sphinx ALL
        COMMAND
        sphinx-build -b html ${CMAKE_CURRENT_LIST_DIR}/docs
        ${CMAKE_CURRENT_BINARY_DIR}/docs
        -Dbreathe_projects.Example=${CMAKE_CURRENT_BINARY_DIR}/xml
        DEPENDS Doxygen
        COMMENT "Generating documentation with Sphinx")

enable_testing()
add_subdirectory(external/doctest)
include(external/doctest/doctest.cmake)
add_subdirectory(src)