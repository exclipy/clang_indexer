cmake_minimum_required(VERSION 2.6)

project (clang_indexer_project)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules/")

find_package(Boost COMPONENTS iostreams REQUIRED)
find_package(ZLIB REQUIRED)
find_package(LibClang REQUIRED)
find_package(BerkeleyDB REQUIRED)

include_directories(
    SYSTEM
    ${Boost_INCLUDE_DIRS}
    ${ZLIB_INCLUDE_DIRS}
    ${LibClang_INCLUDE_DIRS}
    ${DB_INCLUDE_DIRS}
)

add_library(common STATIC clang_index_parser.cpp clang_index_printer.cpp)

add_executable(indexer clang_indexer.cpp)
target_link_libraries(indexer
    common
    ${Boost_LIBRARIES}
    ${ZLIB_LIBRARIES}
    ${LibClang_LIBRARIES}
)

add_executable(linker clang_index_linker.cpp)
target_link_libraries(linker
    common
    ${Boost_LIBRARIES}
    ${ZLIB_LIBRARIES}
)

add_executable(index2db clang_index2db.cpp)
target_link_libraries(index2db
    common
    ${Boost_LIBRARIES}
    ${ZLIB_LIBRARIES}
    ${DB_LIBRARIES}
)
