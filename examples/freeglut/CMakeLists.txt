
if (GLEW_FOUND AND GLUT_FOUND AND OPENGL_FOUND)
    get_filename_component(TARGET_NAME ${CMAKE_CURRENT_SOURCE_DIR} NAME)

    include_directories(
        ${CEF_DIR}
        ${CMAKE_SOURCE_DIR}/src
    )
    link_directories(${CEF_DIR}/lib)

    add_executable(${TARGET_NAME} main.cpp)
    add_dependencies(${TARGET_NAME} SwiftGUI)

    target_link_libraries(${TARGET_NAME}
        SwiftGUI
        ${OPENGL_LIBRARIES}
        ${GLUT_LIBRARIES}
        ${GLEW_LIBRARIES}
    )

    install(TARGETS ${TARGET_NAME}
        RUNTIME DESTINATION "bin"
    )

    install(FILES "data/gui.css" DESTINATION "share")
    install(FILES "data/gui.html" DESTINATION "share")


else (GLEW_FOUND AND GLUT_FOUND AND OPENGL_FOUND)
    message("freeglut is required to build the opengl example")
endif (GLEW_FOUND AND GLUT_FOUND AND OPENGL_FOUND)
