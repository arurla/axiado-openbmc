PACKAGECONFIG = "\
        coredump \
        hostnamed \
        networkd \
        nss \
        pam \
        randomseed \
        resolved \
        seccomp \
        sysusers \
        timedated \
        timesyncd \
        zstd \
        "

EXTRA_OEMESON:append = " -Ddns-servers=''"

PACKAGES =+ "${PN}-catalog-extralocales"

RRECOMMENDS:${PN}:append:openbmc-phosphor = " phosphor-systemd-policy"

FILES:${PN}-catalog-extralocales = "\
    ${exec_prefix}/lib/systemd/catalog/*.*.catalog \
"

ALTERNATIVE_LINK_NAME[init] = "${base_sbindir}/init"
ALTERNATIVE_PRIORITY[init] ?= "300"

ALTERNATIVE:${PN} += "init"
ALTERNATIVE_TARGET[init] = "${rootlibexecdir}/systemd/systemd"
