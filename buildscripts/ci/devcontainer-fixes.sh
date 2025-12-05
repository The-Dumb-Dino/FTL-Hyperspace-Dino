#!/bin/bash
set -e

# Update vcpkg to get latest package versions
(cd /vcpkg && git pull && ./bootstrap-vcpkg.sh)

# Export overlay for Windows MinGW cross-compile boost fix
export VCPKG_OVERLAY_PORTS="/workspaces/FTL-Hyperspace/.devcontainer/vcpkg-overlays"

echo "Devcontainer fixes applied successfully"
