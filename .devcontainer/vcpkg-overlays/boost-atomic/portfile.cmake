# Include the original portfile
include("${VCPKG_ROOT_DIR}/ports/boost-atomic/portfile.cmake")

# Post-install hook: Fix library naming for MinGW cross-compile
# This runs AFTER the package is installed
function(post_install_fix)
    if(VCPKG_CMAKE_SYSTEM_NAME STREQUAL "MinGW")
        file(GLOB_RECURSE boost_libs
            "${CURRENT_PACKAGES_DIR}/lib/*.lib"
            "${CURRENT_PACKAGES_DIR}/debug/lib/*.lib")

        foreach(lib_file ${boost_libs})
            get_filename_component(lib_dir "${lib_file}" DIRECTORY)
            get_filename_component(lib_name "${lib_file}" NAME_WE)

            # Create lib*.a symlink to fix MinGW cross-compile
            file(CREATE_LINK "${lib_name}.lib" "${lib_dir}/lib${lib_name}.a" SYMBOLIC)
            message(STATUS "Created symlink: lib${lib_name}.a -> ${lib_name}.lib")
        endforeach()
    endif()
endfunction()

post_install_fix()
