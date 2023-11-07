#!/usr/bin/env bash

####################################################################################
# Copyright (c) 2021-22 Axiado Corporation (or its affiliates). All rights reserved.
# Use, modification and redistribution of this file is subject to your possession
# of a valid End User License Agreement (EULA) for the Axiado Product of which
# these sources are part of and your compliance with all applicable terms and
# conditions of such licence agreement.
####################################################################################

set -Eeuo pipefail
IFS=$'\n\t'

# Go to project directory
cd "$(dirname "${BASH_SOURCE[0]}")/../.." > /dev/null 2>&1

source ci/scripts/utils.sh

if [[ -x "$(command -v clang-format-13)" ]]; then
	CF=clang-format-13
elif [[ -x "$(command -v clang-format-mp-13)" ]]; then
	# MacPorts version
	CF=clang-format-mp-13
else
	CF=clang-format
fi
print_important "Format all source files using ${CF}"
find . -iname *.h -o -iname *.c -o -iname *.cpp -o -iname *.hpp \
    | xargs clang-format -style=file -i -fallback-style=none

git diff > clang_format.patch

# Delete if 0 size
if [ ! -s clang_format.patch ]
then
    rm clang_format.patch
fi
print_success "Done"
