cmake_minimum_required(VERSION 3.11)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# set the project name
project(pyTORS)

include(FetchContent)

FetchContent_Declare(pybind
  GIT_REPOSITORY https://github.com/pybind/pybind11.git
  GIT_TAG v2.5.0)
FetchContent_GetProperties(pybind)
if(NOT pybind_POPULATED)
  FetchContent_Populate(pybind)
  add_subdirectory(${pybind_SOURCE_DIR} ${pybind_BINARY_DIR} EXCLUDE_FROM_ALL)
endif()

pybind11_add_module(pyTORS MODULE module.cpp)
target_link_libraries(pyTORS PRIVATE cTORS)

if(CMAKE_BUILD_TYPE MATCHES Debug)
    target_link_libraries(pyTORS PRIVATE pybind11::module)
    add_library(restore_default_visibility INTERFACE)
    target_compile_options(restore_default_visibility INTERFACE -fvisibility=default)
    target_link_libraries(pyTORS PRIVATE restore_default_visibility)
endif()

if(${CMAKE_VERSION} VERSION_LESS "3.12.0") 
  find_package(PythonInterp)
  if(PYTHONINTERP_FOUND)
    set(Python_FOUND True)
  endif()
else()
  find_package(Python COMPONENTS Interpreter Development)
endif()

if(Python_FOUND)
  execute_process ( COMMAND ${PYTHON_EXECUTABLE} -c "from distutils.sysconfig import get_python_lib; print(get_python_lib())" OUTPUT_VARIABLE PYTHON_SITE_PACKAGES OUTPUT_STRIP_TRAILING_WHITESPACE)
  message(STATUS "Python found. Intalling to ${PYTHON_SITE_PACKAGES}")
  install(TARGETS pyTORS
  COMPONENT python
  LIBRARY DESTINATION "${PYTHON_SITE_PACKAGES}")
else()
  message(WARNING "Python not found. Python library not installed.")
endif()
  
