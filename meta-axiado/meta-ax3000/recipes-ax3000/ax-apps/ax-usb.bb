# Copyright (c) 2021-22 Axiado Corporation (or its affiliates). All rights reserved.
# Use, modification and redistribution of this file is subject to your possession
# of a valid End User License Agreement (EULA) for the Axiado Product of which
# these sources are part of and your compliance with all applicable terms and
# conditions of such licence agreement.

SUMMARY = "USB Bridging Support"
DESCRIPTION = "USB dev rules and USB configfs script"
LICENSE = "axiado"
PV = "1.0"

LIC_FILES_CHKSUM ?= "file://${WORKDIR}/COPYING.axiado;md5=3da9cfbcb788c80a0384361b4de20420"

S = "${WORKDIR}"

SRC_URI += "file://usbgadget.sh \
	    file://ax-usb.rules \
	    file://COPYING.axiado"

do_install() {
        install -d ${D}/etc/usb/
        install -m 0777 usbgadget.sh ${D}/etc/usb/

        install -d ${D}/etc/udev/rules.d/
        install -m 0644 ax-usb.rules ${D}/etc/udev/rules.d/
}
