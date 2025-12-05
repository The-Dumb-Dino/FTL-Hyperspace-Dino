#!/bin/bash

SCRIPT_DIR=$( cd -- "$( dirname -- "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )
docker run --rm -v $SCRIPT_DIR/../..:/workspaces/FTL-Hyperspace ghcr.io/fr-eed/hs-devcontainer bash -c "cd /workspaces/FTL-Hyperspace && buildscripts/buildall-release-only.sh"
