
set(CEF_RESOURCES
    "${CEF_LIBRARY_DIR}/chrome-sandbox"
    "${CEF_LIBRARY_DIR}/natives_blob.bin"
    "${CEF_LIBRARY_DIR}/snapshot_blob.bin"
)

install(FILES ${CEF_RESOURCES}              DESTINATION "bin")
install(DIRECTORY "${CEF_RESOURCE_DIR}/"    DESTINATION "bin")
install(FILES "gui.css"                     DESTINATION "share")
install(FILES "gui.html"                    DESTINATION "share")
install(DIRECTORY "third-party"             DESTINATION "share")

include_directories(
    ${CEF_DIR}
    ${CMAKE_SOURCE_DIR}/src
)
link_directories(${CEF_DIR}/lib)

add_subdirectory(web_to_tga)
add_subdirectory(freeglut)
add_subdirectory(glfw3)
