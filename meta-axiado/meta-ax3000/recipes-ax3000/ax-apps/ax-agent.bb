FILESEXTRAPATHS:append := "${THISDIR}/${PN}:"

SUMMARY = "ax BMC agent"
SECTION = "Ax BMC agent demo"
LICENSE = "axiado"

LIC_FILES_CHKSUM ?= "file://COPYING.axiado;md5=3da9cfbcb788c80a0384361b4de20420"

SRC_URI =  "file://ax-demo.cpp \
            file://ax-demo-test.cpp \
            file://ax-demo-rw.cpp \
            file://ax-dbus-data.hpp \
            file://ax-dbus-data.cpp \
            file://ax-ipc-endpoints.hpp \
            file://ax-bmc-ipc-device.hpp \
            file://ax-bmc-ipc-device.cpp \
            file://ax-bmc-ipc-main.cpp \
            file://HSMKey-demo-test.cpp \
            file://ax-nw-test.cpp \
            file://demo.service \
            file://test-data.service \
            file://bmc-ipc.service \
            file://meson.build \
            file://meson_options.txt \
            file://COPYING.axiado"

S = "${WORKDIR}"
PV = "1.0"

inherit meson pkgconfig
inherit obmc-phosphor-systemd

DEPENDS = "sdbusplus \
	phosphor-dbus-interfaces \
	sdeventplus \
	phosphor-logging \
	nlohmann-json \
	systemd "

SYSTEMD_SERVICE:${PN} = "demo.service bmc-ipc.service"
