# Copyright (c) 2021-22 Axiado Corporation (or its affiliates). All rights reserved.
# Use, modification and redistribution of this file is subject to your possession
# of a valid End User License Agreement (EULA) for the Axiado Product of which
# these sources are part of and your compliance with all applicable terms and
# conditions of such licence agreement.

SRCBRANCH ?= "release-0.8.0"
SRC_URI = "git://git@github.com/Axiado/webui-vue.git;protocol=ssh;branch=${SRCBRANCH}"
SRCREV = "${AUTOREV}"
PV = "1.0+git${SRCPV}"

python () {
    import os

    WDIR = d.getVar('LOCAL_SRC_PATH', True)
 
    if WDIR and os.path.exists(WDIR) and os.path.isdir(WDIR):
        d.setVar('SRC_URI', 'git://%s/Axiado/webui-vue;protocol=file;branch=${SRCBRANCH}' % WDIR)
        d.appendVar('SRC_URI', ' file://login-company-logo.svg file://logo-header.svg file://favicon.ico')
    else:
        bb.warn("Local directory does not exist: %s" % WDIR)
}

FILESEXTRAPATHS:append := "${THISDIR}/${PN}:"
EXTRA_OENPM:ax3000 = "-- --mode axiado"

SRC_URI += " \
    file://login-company-logo.svg \
    file://logo-header.svg \
    file://favicon.ico \
    "

do_compile:prepend() {
  cp -vf ${WORKDIR}/login-company-logo.svg ${S}/src/assets/images
  cp -vf ${WORKDIR}/logo-header.svg ${S}/src/assets/images
  cp -vf ${WORKDIR}/favicon.ico ${S}/public
}
