
if (GLEW_FOUND AND GLFW_FOUND AND OPENGL_FOUND)
    get_filename_component(TARGET_NAME ${CMAKE_CURRENT_SOURCE_DIR} NAME)

    add_executable(${TARGET_NAME} main.cpp)
    add_dependencies(${TARGET_NAME} SwiftGUI)

    target_link_libraries(${TARGET_NAME}
        SwiftGUI
        ${OPENGL_LIBRARIES}
        ${GLEW_LIBRARIES}
        ${GLFW_LIBRARIES}
    )

    install(TARGETS ${TARGET_NAME}
        RUNTIME DESTINATION "bin"
    )

else (GLEW_FOUND AND GLFW_FOUND AND OPENGL_FOUND)
    message("glfw is required to build the glfw example")
endif (GLEW_FOUND AND GLFW_FOUND AND OPENGL_FOUND)
