include(FindPackageHandleStandardArgs)

set(CEF_DIR "$ENV{CEF_DIR}" CACHE PATH "The base directory of the Chromium embedded Framework")

message("Looking for Chromium Embedded Framework in ${CEF_DIR}")

find_path(CEF_INCLUDE_DIR "include/cef_version.h"
	HINTS ${CEF_DIR})

find_path(CEF_RESOURCE_DIR "cef.pak"
	HINTS ${CEF_DIR}/Resources ${CEF_DIR}/share)

find_path(CEF_LIBRARY_DIR "natives_blob.bin"
	HINTS ${CEF_DIR}/Release ${CEF_DIR}/lib)

find_library(CEF_LIBRARY
	NAMES cef libcef cef.lib libcef.o "Chromium Embedded Framework"
	PATHS ${CEF_DIR} ${CEF_DIR}/Release ${CEF_DIR}/lib)

find_library(CEFWRAPPER_LIBRARY
	NAMES cef_dll_wrapper libcef_dll_wrapper
	PATHS ${CEF_DIR}/build/libcef_dll ${CEF_DIR}/lib)

if (NOT CEF_LIBRARY)
	message(FATAL_ERROR "Could not find the CEF shared library" )
endif (NOT CEF_LIBRARY)

if (NOT CEFWRAPPER_LIBRARY)
	message(FATAL_ERROR "Could not find the CEF wrapper library" )
endif (NOT CEFWRAPPER_LIBRARY)

set(CEF_LIBRARIES
	${CEF_LIBRARY}
	${CEFWRAPPER_LIBRARY})

find_package_handle_standard_args(CEF DEFAULT_MSG CEF_LIBRARY
	CEFWRAPPER_LIBRARY CEF_INCLUDE_DIR)

mark_as_advanced(CEF_LIBRARY CEF_WRAPPER_LIBRARY CEF_LIBRARIES
	CEF_INCLUDE_DIR)
