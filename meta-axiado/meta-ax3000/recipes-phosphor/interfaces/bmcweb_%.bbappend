FILESEXTRAPATHS:prepend := "${THISDIR}/${PN}:"

EXTRA_OEMESON:append: = " \
	-Dredfish-dump-log=enabled \
	-Dredfish-bmc-journal=enabled \
	-Dredfish-cpu-log=enabled \
        -Dredfish-dbus-log=enabled \
        -Drest=enabled \
        -Dhttp-body-limit=65 \
	"

SRC_URI += "file://0001-Add-new-url-HSMKey.patch"
