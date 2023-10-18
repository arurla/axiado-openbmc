# Copyright (c) 2021-22 Axiado Corporation (or its affiliates). All rights reserved.
# Use, modification and redistribution of this file is subject to your possession
# of a valid End User License Agreement (EULA) for the Axiado Product of which
# these sources are part of and your compliance with all applicable terms and
# conditions of such licence agreement.


SUMMARY = "OpenBMC for ax3000 - Applications"
PR = "r1"

inherit packagegroup

PROVIDES = "${PACKAGES}"
PACKAGES = " \
        ${PN}-chassis \
        ${PN}-fans \
        ${PN}-flash \
        ${PN}-system \
        "

PROVIDES += "virtual/obmc-chassis-mgmt"
PROVIDES += "virtual/obmc-fan-mgmt"
PROVIDES += "virtual/obmc-flash-mgmt"
PROVIDES += "virtual/obmc-system-mgmt"

RPROVIDES:${PN}-chassis += "virtual-obmc-chassis-mgmt"
RPROVIDES:${PN}-fans += "virtual-obmc-fan-mgmt"
RPROVIDES:${PN}-flash += "virtual-obmc-flash-mgmt"
RPROVIDES:${PN}-system += "virtual-obmc-system-mgmt"

SUMMARY:${PN}-chassis = "ax3000 Chassis"
RDEPENDS:${PN}-chassis = " \
        x86-power-control \
        "

SUMMARY:${PN}-fans = "ax3000 Fans"
RDEPENDS:${PN}-fans = " \
        phosphor-pid-control \
        "

SUMMARY:${PN}-flash = "ax3000 Flash"
RDEPENDS:${PN}-flash = " \
        phosphor-software-manager \
        "

SUMMARY:${PN}-system = "ax3000 System"
RDEPENDS:${PN}-system = " \
        entity-manager \
        dbus-sensors \
        ipmitool \
        bmcweb \
        webui-vue \
        xz \
        e2fsprogs \
        e2fsprogs-mke2fs \
        util-linux-mkfs \
        ax-agent \
        fru-device \
        net-tools \
        ethtool \
        tcpdump \
        iperf3 \
        usbutils \
        util-linux \
        iproute2 \
        iptables \
        ax-net \
        ax-usb \
        phosphor-dbus-monitor \
        phosphor-image-signing \
        phosphor-host-postd \
        phosphor-post-code-manager \
        strongswan \
        snmp-policy \
        phosphor-snmp \
        ax-utils \
        "
