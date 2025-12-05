#!/bin/bash
set -e

# Update vcpkg to get latest package versions
(cd /vcpkg && git pull && ./bootstrap-vcpkg.sh)

# Pre-install boost-atomic for Windows MinGW cross-compile
# This ensures boost_atomic.lib is available before other packages try to link against it
/vcpkg/vcpkg install boost-atomic:x86-windows-ftl 2>/dev/null || true

# Create symlinks in global vcpkg so they're available during CMake config
# When CMake copies packages into build directories, these symlinks come along
for lib_path in /vcpkg/installed/x86-windows-ftl/{lib,debug/lib}; do
    if [ -d "$lib_path" ] && [ -f "$lib_path/boost_atomic.lib" ]; then
        if [ ! -e "$lib_path/libboost_atomic.a" ]; then
            ln -s boost_atomic.lib "$lib_path/libboost_atomic.a"
        fi
    fi
done


echo "Devcontainer fixes applied successfully"
