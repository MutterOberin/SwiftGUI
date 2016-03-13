set(CEF_RESOURCES
    "${CEF_RESOURCE_DIR}/cef.pak"
    "${CEF_RESOURCE_DIR}/cef_100_percent.pak"
    "${CEF_RESOURCE_DIR}/cef_200_percent.pak"
    "${CEF_RESOURCE_DIR}/cef_extensions.pak"
    "${CEF_RESOURCE_DIR}/devtools_resources.pak"
    "${CEF_RESOURCE_DIR}/icudtl.dat"
)

set(CEF_BINARIES
    "${CEF_BINARY_DIR}/chrome-sandbox"
    "${CEF_BINARY_DIR}/libcef.so"
    "${CEF_BINARY_DIR}/natives_blob.bin"
    "${CEF_BINARY_DIR}/snapshot_blob.bin"
)

file(GLOB_RECURSE CEF_LOCALES ${CEF_RESOURCE_DIR}/locales/*.pak)

install(FILES ${CEF_RESOURCES} DESTINATION "bin")
install(FILES ${CEF_BINARIES} DESTINATION "bin")
install(FILES ${CEF_LOCALES} DESTINATION "bin/locales")

add_subdirectory(simple)
