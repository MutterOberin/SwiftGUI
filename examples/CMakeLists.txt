
set(CEF_RESOURCES
    "${CEF_DIR}/share/cef.pak"
    "${CEF_DIR}/share/cef_100_percent.pak"
    "${CEF_DIR}/share/cef_200_percent.pak"
    "${CEF_DIR}/share/cef_extensions.pak"
    "${CEF_DIR}/share/devtools_resources.pak"
    "${CEF_DIR}/share/icudtl.dat"
    "${CEF_DIR}/lib/chrome-sandbox"
    "${CEF_DIR}/lib/libcef.so"
    "${CEF_DIR}/lib/natives_blob.bin"
    "${CEF_DIR}/lib/snapshot_blob.bin"
)

file(GLOB_RECURSE CEF_LOCALES ${CEF_DIR}/share/locales/*.pak)

install(FILES ${CEF_RESOURCES} DESTINATION "bin")
install(FILES ${CEF_LOCALES} DESTINATION "bin/locales")

add_subdirectory(simple)
