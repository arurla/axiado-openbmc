# Copyright (c) 2021-22 Axiado Corporation (or its affiliates). All rights reserved.
# Use, modification and redistribution of this file is subject to your possession
# of a valid End User License Agreement (EULA) for the Axiado Product of which
# these sources are part of and your compliance with all applicable terms and
# conditions of such licence agreement.

SUMMARY = "Axiado TCU Utils"
DESCRIPTION = "Axiado TCu Utils"
#LICENSE = "axiado"
LICENSE = "CLOSED"
PV = "1.0"

#LIC_FILES_CHKSUM ?= "file://${WORKDIR}/COPYING.axiado;md5=3da9cfbcb788c80a0384361b4de20420"

S = "${WORKDIR}"

SRC_URI += "file://flowmeter/flowmeter.service \
            file://flowmeter/flowmeter \
            file://flowmeter/rwui.service \
            file://flowmeter/ransomware-predictor\
            file://logmgr/ax_logger.service \
            file://logmgr/ax_logger \
            file://logmgr/ax_logger.conf \
            file://sysproxy/sysproxy.service \
            file://sysproxy/sysmgr_proxy \
            file://tdfu/ax200-fw-upgrade \
            "
#SRC_URI += "file://flowmeter/flowmeter.service "
#SRC_URI += "file://obmc-phosphor-image-ax3000.wic.xz;unpack=0 \
#            file://flash.sh"
#SRC_URI += "file://flash.sh "

FILES:${PN} += "/usr/bin/flowmeter"
FILES:${PN} += "/etc/systemd/system/multi-user.target.wants/flowmeter.service"
FILES:${PN} += "/lib/systemd/system/flowmeter.service"
FILES:${PN} += "/usr/bin/ransomware-predictor"
FILES:${PN} += "/etc/systemd/system/multi-user.target.wants/rwui.service"
FILES:${PN} += "/lib/systemd/system/rwui.service"
FILES:${PN} += "/usr/bin/ax_logger"
FILES:${PN} += "/etc/systemd/system/multi-user.target.wants/ax_logger.service"
FILES:${PN} += "/lib/systemd/system/ax_logger.service"
FILES:${PN} += "/etc/ax_logger.conf"
FILES:${PN} += "/usr/bin/sysmgr_proxy"
FILES:${PN} += "/etc/systemd/system/multi-user.target.wants/sysproxy.service"
FILES:${PN} += "/lib/systemd/system/sysproxy.service"
FILES:${PN} += "/usr/bin/ax200_fw_upgrade"

#Required for flowmeter
DEPENDS += "libpcap systemd"

do_install() {
        install -d ${D}/lib/systemd/system/
        install -d ${D}/usr/bin/
        install -d ${D}/etc/systemd/system/multi-user.target.wants/
        #Flowmeter
        cp ${WORKDIR}/flowmeter/flowmeter.service ${D}/lib/systemd/system/
        ln -s -r ${D}/lib/systemd/system/flowmeter.service ${D}/etc/systemd/system/multi-user.target.wants/flowmeter.service
        install -m 0755 ${WORKDIR}/flowmeter/flowmeter ${D}/usr/bin/
        #Ransomware
        cp ${WORKDIR}/flowmeter/rwui.service ${D}/lib/systemd/system/
        ln -s -r ${D}/lib/systemd/system/rwui.service ${D}/etc/systemd/system/multi-user.target.wants/rwui.service
        install -m 0755 ${WORKDIR}/flowmeter/ransomware-predictor ${D}/usr/bin/
        #Logmgr
        cp ${WORKDIR}/logmgr/ax_logger.service ${D}/lib/systemd/system/
        cp ${WORKDIR}/logmgr/ax_logger.conf ${D}/etc/
        ln -s -r ${D}/lib/systemd/system/ax_logger.service ${D}/etc/systemd/system/multi-user.target.wants/ax_logger.service
        install -m 0755 ${WORKDIR}/logmgr/ax_logger ${D}/usr/bin/
        #sysproxy
        cp ${WORKDIR}/sysproxy/sysproxy.service ${D}/lib/systemd/system/
        ln -s -r ${D}/lib/systemd/system/sysproxy.service ${D}/etc/systemd/system/multi-user.target.wants/sysproxy.service
        install -m 0755 ${WORKDIR}/sysproxy/sysmgr_proxy ${D}/usr/bin/
        #tdfu
        install -m 0755 ${WORKDIR}/tdfu/ax200-fw-upgrade ${D}/usr/bin/
}

