cmake_minimum_required( VERSION 2.8 )
project( Simple-Function-Curves )

set( CMAKE_C_STANDARD 99 )
set( CMAKE_C_STANDARD_REQUIRED ON )

set( LIBRARY_DIR ${CMAKE_CURRENT_LIST_DIR} CACHE PATH "Relative or absolute path to directory where built shared libraries will be placed" )

add_library( FunctionCurves SHARED ${CMAKE_CURRENT_LIST_DIR}/curves.c )
set_target_properties( FunctionCurves PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${LIBRARY_DIR} )
target_include_directories( FunctionCurves PUBLIC ${CMAKE_CURRENT_LIST_DIR} )
target_compile_definitions( FunctionCurves PUBLIC -DDEBUG )
