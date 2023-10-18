# Copyright (c) 2021-22 Axiado Corporation (or its affiliates). All rights reserved.
# Use, modification and redistribution of this file is subject to your possession
# of a valid End User License Agreement (EULA) for the Axiado Product of which
# these sources are part of and your compliance with all applicable terms and
# conditions of such licence agreement.

DESCRIPTION = "Linux kernel for Axiado"
SECTION = "kernel"
LICENSE = "axiado"

LIC_FILES_CHKSUM ?= "file://COPYING;md5=6bc538ed5bd9a7fc9398086aedcd7e46"

PROVIDES += "virtual/kernel"

KCONFIG_MODE="--alldefconfig"

inherit kernel_axiado
require recipes-kernel/linux/linux-yocto.inc

SRCBRANCH ?= "release-0.8.0"
SRCREV = "${AUTOREV}"
SRC_URI = "git://git@github.com/Axiado/kernel.git;protocol=ssh;branch=${SRCBRANCH}"

python () {
    import os

    KDIR = d.getVar('LOCAL_SRC_PATH', True)

    if KDIR and os.path.exists(KDIR) and os.path.isdir(KDIR):
        d.setVar('SRC_URI', 'git://%s/Axiado/kernel;protocol=file;branch=${SRCBRANCH}' % KDIR)
    else:
        bb.warn("Directory does not exist: %s" % KDIR)
}

LINUX_VERSION ?= "5.15.80"
PV = "${LINUX_VERSION}+git${SRCPV}"

S = "${WORKDIR}/git"
B = "${WORKDIR}/build"

KBUILD_DEFCONFIG = "axiado_defconfig"
