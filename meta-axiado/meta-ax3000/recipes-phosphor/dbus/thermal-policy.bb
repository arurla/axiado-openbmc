SUMMARY = "Thermal policy configuration for meta-ax200 machines"
LICENSE = "axiado"

LIC_FILES_CHKSUM ?= "file://COPYING.axiado;md5=3da9cfbcb788c80a0384361b4de20420"

inherit allarch
inherit phosphor-dbus-monitor

FILESEXTRAPATHS:prepend := "${THISDIR}/${BPN}:"

SRC_URI:append = " file://config.yaml \
                   file://COPYING.axiado"
S = "${WORKDIR}"
PV = "1.0"

do_install() {
        install -D ${WORKDIR}/config.yaml ${D}${config_dir}/config.yaml
}


FILES:${PN}:append = " ${config_dir}/config.yaml"
