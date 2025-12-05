#!/bin/bash
set -e

# Update vcpkg to get latest package versions
(cd /vcpkg && git pull && ./bootstrap-vcpkg.sh)

# Try to install all packages to both Windows build directories
# boost-filesystem will fail due to boost-atomic lib naming, that's expected
/vcpkg/vcpkg install --triplet=x86-windows-ftl --x-install-root=build-windows-debug/vcpkg_installed || true
/vcpkg/vcpkg install --triplet=x86-windows-ftl --x-install-root=build-windows-release/vcpkg_installed || true

# Fix boost_atomic.lib naming in both build directories so filesystem can link
for build_dir in build-windows-debug build-windows-release; do
    cp "$build_dir/vcpkg_installed/x86-windows-ftl/lib/boost_atomic.lib" "$build_dir/vcpkg_installed/x86-windows-ftl/lib/libboost_atomic.a"
    cp "$build_dir/vcpkg_installed/x86-windows-ftl/debug/lib/boost_atomic.lib" "$build_dir/vcpkg_installed/x86-windows-ftl/debug/lib/libboost_atomic.a"
done

echo "Devcontainer fixes applied successfully"
