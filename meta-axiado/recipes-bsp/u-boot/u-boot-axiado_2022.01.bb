# Copyright (c) 2021-22 Axiado Corporation (or its affiliates). All rights reserved.
# Use, modification and redistribution of this file is subject to your possession
# of a valid End User License Agreement (EULA) for the Axiado Product of which
# these sources are part of and your compliance with all applicable terms and
# conditions of such licence agreement.

SUMMARY = "Axiado uboot recipe"
SECTION = "bootloaders"
LICENSE = "axiado"
LIC_FILES_CHKSUM = "file://README;beginline=1;endline=4;md5=744e7e3bb0c94b4b9f6b3db3bf893897"

require recipes-bsp/u-boot/u-boot.inc
PROVIDES += "u-boot"

SRCBRANCH ?= "release-0.8.0"
SRCREV ?= "${AUTOREV}"
SRC_URI = "git://git@github.com/Axiado/u-boot.git;protocol=ssh;branch=${SRCBRANCH}"


S = "${WORKDIR}/git"
B = "${WORKDIR}/build"

DEPENDS += " dtc-native bison-native"
