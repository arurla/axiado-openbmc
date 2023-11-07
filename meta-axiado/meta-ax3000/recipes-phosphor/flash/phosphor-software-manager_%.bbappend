FILESEXTRAPATHS:prepend := "${THISDIR}/${PN}:"

SRC_URI:append = "\
    file://0001-add-sw-version.patch \
    file://0001-changes-in-BMC-update-script-for-ax200.patch \
    file://0001-start-sysmgr-image-update-app.patch \
"
