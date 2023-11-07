SUMMARY = "SNMP policy configuration for meta-ax200 machines"
LICENSE = "axiado"

LIC_FILES_CHKSUM ?= "file://COPYING.axiado;md5=3da9cfbcb788c80a0384361b4de20420"

inherit allarch
inherit phosphor-dbus-monitor

FILESEXTRAPATHS:prepend := "${THISDIR}/${BPN}:"

SRC_URI:append = " file://snmp-policy.yaml \
                   file://COPYING.axiado"
S = "${WORKDIR}"
PV = "1.0"

do_install() {
        install -D ${WORKDIR}/snmp-policy.yaml ${D}${config_dir}/snmp-policy.yaml
}


FILES:${PN}:append = " ${config_dir}/snmp-policy.yaml"
