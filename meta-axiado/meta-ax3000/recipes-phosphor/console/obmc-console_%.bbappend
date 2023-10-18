FILESEXTRAPATHS:prepend := "${THISDIR}/${PN}:"
OBMC_CONSOLE_HOST_TTY = "ttyPS1"

SRC_URI += "file://server.ttyPS1.conf"
SRC_URI += "file://80-obmc-console-uart.rules"

do_install:append() {
        install -d ${D}/${nonarch_base_libdir}/udev/rules.d
        install -m 0644 ${WORKDIR}/80-obmc-console-uart.rules ${D}/${nonarch_base_libdir}/udev/rules.d
}
