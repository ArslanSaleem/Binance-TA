# This module tries to find libWebsockets library and include files
#
# LIBWEBSOCKETS_INCLUDE_DIR, path where to find libwebsockets.h
# LIBWEBSOCKETS_LIBRARY_DIR, path where to find libwebsockets.so
# LIBWEBSOCKETS_LIBRARIES, the library to link against
# LIBWEBSOCKETS_FOUND, If false, do not try to use libWebSockets
#
# This currently works probably only for Linux

include(FindPackageHandleStandardArgs)
set(FRESH_DOWNLOAD off CACHE BOOL "download a fresh copy of all dependencies")
include(ExternalProject)
SET ( LIBWEBSOCKETS_FOUND FALSE )

FIND_PATH ( LIBWEBSOCKETS_INCLUDE_DIR NAMES libwebsockets.h
    HINTS /usr/local/include /usr/include
)

FIND_LIBRARY ( LIBWEBSOCKETS_LIBRARIES NAMES websockets
    HINTS /usr/local/lib /usr/lib
)

GET_FILENAME_COMPONENT( LIBWEBSOCKETS_LIBRARY_DIR ${LIBWEBSOCKETS_LIBRARIES} PATH )

IF ( LIBWEBSOCKETS_INCLUDE_DIR )
    IF ( LIBWEBSOCKETS_LIBRARIES )
        SET ( LIBWEBSOCKETS_FOUND TRUE )
    ENDIF ( LIBWEBSOCKETS_LIBRARIES )
ENDIF ( LIBWEBSOCKETS_INCLUDE_DIR )


IF ( LIBWEBSOCKETS_FOUND )
    MARK_AS_ADVANCED(
        LIBWEBSOCKETS_LIBRARY_DIR
        LIBWEBSOCKETS_INCLUDE_DIR
        LIBWEBSOCKETS_LIBRARIES
    )
ENDIF ( )

add_library(libwebsockets UNKNOWN IMPORTED)
set_target_properties(libwebsockets PROPERTIES
    INTERFACE_INCLUDE_DIRECTORIES ${LIBWEBSOCKETS_INCLUDE_DIR}
    IMPORTED_LOCATION ${LIBWEBSOCKETS_LIBRARIES}
)

find_package_handle_standard_args(LibWebSockets
	DEFAULT_MSG
	LIBWEBSOCKETS_INCLUDE_DIR
	LIBWEBSOCKETS_LIBRARIES)

