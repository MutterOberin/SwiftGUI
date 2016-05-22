get_filename_component(TARGET_NAME ${CMAKE_CURRENT_SOURCE_DIR} NAME)

add_executable(${TARGET_NAME} main.cpp)
add_dependencies(${TARGET_NAME} SwiftGUI)

target_link_libraries(${TARGET_NAME} SwiftGUI)

install(TARGETS ${TARGET_NAME}
  RUNTIME DESTINATION "bin"
)
