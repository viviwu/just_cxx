
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 11)

file(GLOB_RECURSE SQLITE3_SRC ../common/3rd/*)

add_executable(data_store
        main.cpp
        data_store.h
        data_store.cpp
        ${SQLITE3_SRC}
        site_store.cpp
        site_store.h
        data_def.h
)

target_include_directories(data_store PRIVATE
        ../common/3rd/
)

message("SQLITE3_SRC: " ${SQLITE3_SRC})