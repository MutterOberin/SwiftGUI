################################################################
# Build the library
################################################################

set(TARGET_NAME SwiftGUI)

file(GLOB_RECURSE FILES_SRC RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
    "*.cpp"
)

file(GLOB FILES_PUBLIC_HEADERS RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
    "SwiftGUI/*.hpp"
)


include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CEF_DIR}
)

link_directories(${CEF_DIR}/lib)

add_library(${TARGET_NAME} SHARED ${FILES_SRC})

target_link_libraries(${TARGET_NAME} cef cef_dll_wrapper)

install(FILES ${FILES_PUBLIC_HEADERS} DESTINATION "include/SwiftGUI")
install(TARGETS ${TARGET_NAME}
  RUNTIME DESTINATION bin
  LIBRARY DESTINATION lib
  ARCHIVE DESTINATION lib
)
