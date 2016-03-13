get_filename_component(TARGET_NAME ${CMAKE_CURRENT_SOURCE_DIR} NAME)

include_directories(
    ${CEF_INCLUDE_DIRS}
    ${CMAKE_SOURCE_DIR}/src
)
link_directories(${CEF_LIBRARY_DIRS})

add_executable(${TARGET_NAME} main.cpp)
add_dependencies(${TARGET_NAME} SwiftGUI)

target_link_libraries(${TARGET_NAME} SwiftGUI)

install(FILES ${TARGET_NAME}.sh DESTINATION "bin" PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE)
install(TARGETS ${TARGET_NAME}
  RUNTIME DESTINATION bin
)
