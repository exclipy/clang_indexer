find_path(DB_INCLUDE_DIR db_cxx.h
          PATH_SUFFIXES db51 db52)

find_library(DB_LIBRARY NAMES db_cxx
             PATH_SUFFIXES db51 db52)

set(DB_LIBRARIES ${DB_LIBRARY})
set(DB_INCLUDE_DIRS ${DB_INCLUDE_DIR})

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(DB DEFAULT_MSG DB_LIBRARY DB_INCLUDE_DIR)

mark_as_advanced(DB_INCLUDE_DIR DB_LIBRARY)
