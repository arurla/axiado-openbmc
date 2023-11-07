#!/bin/bash
set -e
. setup ax3000
touch conf/sanity.conf

echo "DL_DIR ?= \"/User_data/home/jenkins/openbmc/downloads\"" >> conf/local.conf
echo "LOCAL_SRC_PATH = \"$1\"" >> conf/local.conf

bitbake obmc-phosphor-image
