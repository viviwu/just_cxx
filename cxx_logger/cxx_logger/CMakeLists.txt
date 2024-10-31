cmake_minimum_required(VERSION 3.5)

project(cxx_logger LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# if(${QT_VERSION} VERSION_LESS 6.0.0)
    if(MSVC)
        message("Tell the compiler(MSVC) that cpp source code is encoded in utf-8!")
        add_compile_options("/source-charset:utf-8")
        #    add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/source-charset:utf-8>")
    endif()
# endif()

set(PKG_INSTALL_PATH ${CMAKE_CURRENT_SOURCE_DIR}/bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PKG_INSTALL_PATH})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PKG_INSTALL_PATH})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PKG_INSTALL_PATH})
set(EXECUTABLE_OUTPUT_PATH ${PKG_INSTALL_PATH})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${PKG_INSTALL_PATH})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${PKG_INSTALL_PATH})

add_executable(cxx_logger main.cpp)

include(GNUInstallDirs)
install(TARGETS cxx_logger
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
