FILESEXTRAPATHS:prepend := "${THISDIR}/${PN}:"

SRC_URI:append = "\
    file://0001-Add-meson-file-for-Axiado-yaml.patch \
    file://0002-Add-Axiado-yaml-for-demo-agent.patch \
    file://0003-Add-Axiado-yaml-for-HSM-Key.patch \
"
