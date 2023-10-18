#!/bin/bash
set -e

. setup ax3000
touch conf/sanity.conf
bitbake obmc-phosphor-image
