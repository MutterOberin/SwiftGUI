
set(CEF_RESOURCES
    "${CEF_LIBRARY_DIR}/chrome-sandbox"
    "${CEF_LIBRARY_DIR}/natives_blob.bin"
    "${CEF_LIBRARY_DIR}/snapshot_blob.bin"
)

install(FILES ${CEF_RESOURCES} DESTINATION "bin")
install(DIRECTORY "${CEF_RESOURCE_DIR}/" DESTINATION "bin")
install(DIRECTORY "third-party" DESTINATION "share")

add_subdirectory(web_to_tga)
add_subdirectory(freeglut)
